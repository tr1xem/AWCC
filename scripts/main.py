#!/usr/bin/env python3
import argparse
import logging
import os
import signal
import subprocess
import sys
import time
from pathlib import Path
from threading import Thread

import evdev


class G15KeyHandler:
    """Handler for G15 special key events."""

    def __init__(self, device_name="AT Translated Set 2 keyboard", timeout=0.3):
        self.device_name = device_name
        self.timeout = timeout
        self.device = None
        self.running = True
        self.last_triggered = {}

        # Use XDG config directory or fallback to ~/.config
        config_dir = (
            Path(os.environ.get("XDG_CONFIG_HOME", "~/.config")).expanduser()
            / "g15-handler"
        )
        config_dir.mkdir(parents=True, exist_ok=True)
        self.state_file = config_dir / "brightness_state.txt"
        self.gmode_state_file = config_dir / "gmode_state.txt"

        # Set up signal handlers for graceful shutdown
        signal.signal(signal.SIGTERM, self._signal_handler)
        signal.signal(signal.SIGINT, self._signal_handler)

    def _signal_handler(self, signum, frame):
        """Handle shutdown signals gracefully."""
        logging.info(f"Received signal {signum}, shutting down...")
        self.running = False
        if self.device:
            self.device.close()
        sys.exit(0)

    def run_script_async(
        self,
        script_path,
        args=None,
        notification_title=None,
        notification_body=None,
        notification_icon=None,
    ):
        """Run a script asynchronously in a separate thread."""
        if args is None:
            args = []

        def _runner():
            try:
                subprocess.run(
                    [script_path] + args, check=True, capture_output=True, text=True
                )
                logging.info(f"Executed: {script_path} {' '.join(args)}")
                if notification_title:
                    cmd = ["notify-send", notification_title]
                    if notification_body:
                        cmd.append(notification_body)
                    if notification_icon:
                        cmd.extend(["-i", notification_icon])
                    subprocess.run(cmd, check=False, capture_output=True)
            except subprocess.CalledProcessError as e:
                logging.error(f"Failed to execute {script_path}: {e}")
                if notification_title:
                    subprocess.run(
                        ["notify-send", notification_title, "Error executing command"],
                        check=False,
                        capture_output=True,
                    )
            except FileNotFoundError:
                logging.error(f"Command not found: {script_path}")

        Thread(target=_runner, daemon=True).start()

    def get_next_brightness(self):
        """Get next brightness value in cycle: 0 → 50 → 100 → 0."""
        try:
            with open(self.state_file, "r") as f:
                current = int(f.read().strip())
        except (FileNotFoundError, ValueError):
            current = 0

        # Cycle through brightness values
        if current == 0:
            next_val = 50
        elif current == 50:
            next_val = 100
        else:  # current == 100 or any other value
            next_val = 0

        # Save next state
        try:
            with open(self.state_file, "w") as f:
                f.write(str(next_val))
        except OSError as e:
            logging.error(f"Failed to save brightness state: {e}")

        return next_val

    def get_next_gmode_state(self):
        """Get next G-Mode state and return (next_state, message, icon)."""
        try:
            with open(self.gmode_state_file, "r") as f:
                current = f.read().strip().lower() == "true"
        except (FileNotFoundError, ValueError):
            current = False

        # Toggle state
        next_state = not current

        # Save next state
        try:
            with open(self.gmode_state_file, "w") as f:
                f.write(str(next_state).lower())
        except OSError as e:
            logging.error(f"Failed to save G-Mode state: {e}")

        if next_state:
            message = "Turning on G-Mode"
            icon = "power-profile-performance-symbolic"
        else:
            message = "Turning off G-Mode"
            icon = "power-profile-balanced-symbolic"

        return next_state, message, icon

    def find_device(self):
        """Find and return the target input device."""
        try:
            devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
            for dev in devices:
                if dev.name == self.device_name:
                    logging.info(f"Found device: {dev.path} ({dev.name})")
                    return dev

            logging.error(f"Device '{self.device_name}' not found")
            available = [dev.name for dev in devices]
            logging.info(f"Available devices: {', '.join(available)}")
            return None

        except Exception as e:
            logging.error(f"Error finding device: {e}")
            return None

    def handle_event(self, event):
        """Handle a single input event."""
        if event.type != evdev.ecodes.EV_MSC:
            return

        event_value = event.value
        now = time.time()

        # Debounce check
        if (now - self.last_triggered.get(event_value, 0)) < self.timeout:
            return

        # Set time immediately before processing
        self.last_triggered[event_value] = now

        # Handle specific key events
        if event_value == 105:  # Brightness key
            brightness = self.get_next_brightness()
            logging.info(f"Brightness key pressed, setting brightness to {brightness}%")
            self.run_script_async("awcc", ["brightness", str(brightness)])
        elif event_value == 104:  # Gaming mode key
            gmode_state, gmode_message, gmode_icon = self.get_next_gmode_state()
            logging.info(f"Gaming mode key pressed: {gmode_message}")
            self.run_script_async(
                "awcc",
                ["gt"],
                notification_title="Alienware Command Centre",
                notification_body=gmode_message,
                notification_icon=gmode_icon,
            )

    def run(self):
        """Main event loop."""
        self.device = self.find_device()
        if not self.device:
            return False

        logging.info(f"Listening for events on: {self.device.path}")

        try:
            for event in self.device.read_loop():
                if not self.running:
                    break
                self.handle_event(event)
        except Exception as e:
            logging.error(f"Error in event loop: {e}")
            return False
        finally:
            if self.device:
                self.device.close()

        return True


def setup_logging(verbose=False):
    """Configure logging."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(
        level=level,
        format="%(asctime)s - %(levelname)s - %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )


def parse_arguments():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(
        description="G15 Key Event Handler for Dell G15 Gaming Laptops",
        epilog="This daemon listens for special key events and executes AWCC commands.",
    )
    parser.add_argument(
        "--verbose", "-v", action="store_true", help="Enable verbose logging"
    )
    parser.add_argument(
        "--device",
        "-d",
        default="AT Translated Set 2 keyboard",
        help="Input device name to monitor (default: %(default)s)",
    )
    parser.add_argument(
        "--timeout",
        "-t",
        type=float,
        default=0.3,
        help="Debounce timeout in seconds (default: %(default)s)",
    )
    return parser.parse_args()


def main():
    """Main function."""
    args = parse_arguments()
    setup_logging(args.verbose)

    handler = G15KeyHandler(device_name=args.device, timeout=args.timeout)

    try:
        success = handler.run()
        sys.exit(0 if success else 1)
    except KeyboardInterrupt:
        logging.info("Interrupted by user")
        sys.exit(0)
    except Exception as e:
        logging.error(f"Unexpected error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
