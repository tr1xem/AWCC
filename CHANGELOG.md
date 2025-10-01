# Changelog

## [1.1.0](https://github.com/tr1xem/AWCC/compare/v1.0.3...v1.1.0) (2025-10-01)


### 🚀 New Features

* **acpi:** exit instead of crash ([dab685f](https://github.com/tr1xem/AWCC/commit/dab685ff0bc2aa70e8fad4adc10a135cd41abe9e))
* **acpi:** Fallback deviceinfo for adding new mode ([a4a196c](https://github.com/tr1xem/AWCC/commit/a4a196cd566eaf5c01ce0611e581f58abfbb7622))
* **autorealse:** testing ([f6d74c0](https://github.com/tr1xem/AWCC/commit/f6d74c0866d79ddd44b55b2e58c9675a8cf5a1d8))
* **awcc:** follow standard c++ project structure ([42b66a5](https://github.com/tr1xem/AWCC/commit/42b66a5bee8ef579e8fd7cc028cfe654fe13b5e8))
* **awcc:** use new device detection API and merge AWCC interface ([72d41d3](https://github.com/tr1xem/AWCC/commit/72d41d38b38909e9e550998e61e161bc4cc41f38))
* **clangtidy:** add clang-tidy ([875e215](https://github.com/tr1xem/AWCC/commit/875e215aff7c2d0c54de942dec12b6b92ab141ed))
* **cli:** added full cli mode support ([c9eb07e](https://github.com/tr1xem/AWCC/commit/c9eb07e2cde7e75480a312dfaff756fffb81f8fc))
* **cli:** turbo ([8936a19](https://github.com/tr1xem/AWCC/commit/8936a19c81856a84a3b810f725b03d9a4f30c36d))
* **daemon:** add daemon support ([caff99f](https://github.com/tr1xem/AWCC/commit/caff99f93782d00aa805a22be0fbddd5476bbfaf))
* **daemon:** sanatize commands first ([939022b](https://github.com/tr1xem/AWCC/commit/939022b4657a65523cc11999e05272d624bd50a6))
* **device_detection:** add support for multiple devices ([0b617af](https://github.com/tr1xem/AWCC/commit/0b617aff2cd26689a73164acafac9d6238875702))
* **device_resolution:** Initialize ACPI in correct order and Handel ([4b2bc61](https://github.com/tr1xem/AWCC/commit/4b2bc61aec1657717cf594846c41e42730e08f09))
* **effectsfx:** added all effects and keybinds ([0e2005b](https://github.com/tr1xem/AWCC/commit/0e2005ba638712cf50c4ad2bcf4d9fdc775aa87c))
* **gkey:** gkey support ([11151d8](https://github.com/tr1xem/AWCC/commit/11151d883468a9df22154b4ecd548ae1fdf5167e))
* **gmode:** added Gmode flag ([76f1fb4](https://github.com/tr1xem/AWCC/commit/76f1fb4cfd757050a4a815eae41bb958e14a130a))
* **gui:** fallback fonts and remove print ([d1bf5dc](https://github.com/tr1xem/AWCC/commit/d1bf5dcf03356b21a72e682d1ea972f1aa43bf41))
* **GUI:** Final GUI Push ([3b8569d](https://github.com/tr1xem/AWCC/commit/3b8569d693ee94bc320a2692224e88eeb95cfffe))
* **gui:** GUI with initial features ([b54371e](https://github.com/tr1xem/AWCC/commit/b54371ece0635ff7724a0e4f85bcc8a4375e341f))
* **hacking:** new commands ([6727544](https://github.com/tr1xem/AWCC/commit/67275441527dff1ca6cd957b8e125374fbccfcdd))
* **install:** cmake install script ([5b10bf9](https://github.com/tr1xem/AWCC/commit/5b10bf99c221afef3b0d0193cca7a4178a4b18be))
* **install:** install script for testers ([bc2fab6](https://github.com/tr1xem/AWCC/commit/bc2fab6fe3c1daa5ba4971b9d8b3c10b2df5d944))
* **keybinder:** add keybinder module ([27f3c91](https://github.com/tr1xem/AWCC/commit/27f3c9118a86380a111d2f361384869a3a629dd3))
* **lighfx:** added lightfx module ([59a35a7](https://github.com/tr1xem/AWCC/commit/59a35a74952a69ab9d82a2df9a816a3fa0247e61))
* **release:** config ([771a553](https://github.com/tr1xem/AWCC/commit/771a5538c7c75fc9497423d97851df4485468b44))
* **sockets:** intial sockets ([48a960f](https://github.com/tr1xem/AWCC/commit/48a960f580a7a4e24223168b8d526a4a71ef9e96))
* **superboost:** basic merge of superboost ([541fcc0](https://github.com/tr1xem/AWCC/commit/541fcc01167271e2c50ced24f4df25fbb1b670a3))
* **thermal:** add support for cpu check ([a4aa49c](https://github.com/tr1xem/AWCC/commit/a4aa49c50026564fdada0d1cb044de6da4ba9896))
* **thermal:** querymode and toggleGmode ([f8f7d81](https://github.com/tr1xem/AWCC/commit/f8f7d8194aec9cf71cc23775d2e66713f545d9be))
* **thermals:** new methods ([f883ed6](https://github.com/tr1xem/AWCC/commit/f883ed6ff3713ff16996356de1649854afbe3a4c))
* **workflow:** realease workflow ([fa0ab26](https://github.com/tr1xem/AWCC/commit/fa0ab26d95c3f67a5484057ad3697ed1a8792767))
* **workflow:** realease workflow ([d8fe7bf](https://github.com/tr1xem/AWCC/commit/d8fe7bf2adeb5370dd6201bd9fb208b347446107))
* **workflows:** auto set project version to release tag ([86bf9fd](https://github.com/tr1xem/AWCC/commit/86bf9fdf65d6a64108348d15a3cfb261987a6396))


### 🐛 Bug Fixes

* **acpi detection:** removed acip_call utility ([0f2faa0](https://github.com/tr1xem/AWCC/commit/0f2faa00c39c4a83127795b0c6f49ae7fa530a5f))
* **acpi:** return hex value ([8530b9f](https://github.com/tr1xem/AWCC/commit/8530b9fd9788e297c880bbcc45d5cc16f7214169))
* **daemon:** already in mode added ([b349976](https://github.com/tr1xem/AWCC/commit/b34997673c2f546f3ebfb7212ed0d643b68c0628))
* **daemon:** already in mode added ([b01d00b](https://github.com/tr1xem/AWCC/commit/b01d00bf654ee3cca5e88a59f737fad43775b89e))
* **daemon:** remove device detection logic ([831672d](https://github.com/tr1xem/AWCC/commit/831672d7e61314bd35cd38d9ad19c5d6afeeccbe))
* **daemon:** remove device detection logic ([d3665f4](https://github.com/tr1xem/AWCC/commit/d3665f4a830eed2ab139031bf3c24b09e7298d3f))
* **device-info:** sudo fallback for acip ([5e1416a](https://github.com/tr1xem/AWCC/commit/5e1416aff21231b90e90b6f9a8c49527d9d6e7d8))
* **device-info:** sudo fallback for acip ([c621a24](https://github.com/tr1xem/AWCC/commit/c621a24dcba8b9ebccfbc6154c985df586f78af0))
* **gmode:** fix gmode switching and toggle ([6fe5a08](https://github.com/tr1xem/AWCC/commit/6fe5a08eed34d7c892142b7ac235882cb3a8c95d))
* **light:** always send 33-byte zero-padded packets ([7fc57e5](https://github.com/tr1xem/AWCC/commit/7fc57e561734e54c1861c62dc59271cd1e506f71))
* **light:** strip to 33 byte & imgui enhance ([635ea0a](https://github.com/tr1xem/AWCC/commit/635ea0a8f21c7336ab6b8fd0c8fe816ee13ca3f9))
* **light:** use dmi and usb detection ([b2b31fc](https://github.com/tr1xem/AWCC/commit/b2b31fc02546ccbe4a3500d2449e096f037ef41f))


### ⚙️ Chores

* **main:** release 1.1.0 ([8017b60](https://github.com/tr1xem/AWCC/commit/8017b6082766ba114d91e90c3cb4722dda93a965))
* **main:** release 1.1.0 ([a475fd4](https://github.com/tr1xem/AWCC/commit/a475fd4c30852535f22200d6907263b67d754085))


### ♻️ Code Refactoring

* **acpi:** print acpicode & g16 7630 support ([f07fff4](https://github.com/tr1xem/AWCC/commit/f07fff46b6bb0ce59a155115d3897b32a3b59302))
* **acpi:** remove the need of acpiModelID ([4262bb0](https://github.com/tr1xem/AWCC/commit/4262bb0f5bd825c1f3ef069220549edad6a60df2))
* **acpiutils:** Instanciate acpiutil only once ([a3e7edf](https://github.com/tr1xem/AWCC/commit/a3e7edf9d4e11b696c069bcd89c1ced8bb06ce8e))
* **awcc:** structs ([74d3811](https://github.com/tr1xem/AWCC/commit/74d3811f6c20819b592874912951d452da59c6b9))
* **daemon:** use callbacks & gmode key ([b3c88b2](https://github.com/tr1xem/AWCC/commit/b3c88b2034434624ae306ef1db83e5d496179c49))
* **database:** enum class instead of enum ([23e33a9](https://github.com/tr1xem/AWCC/commit/23e33a98926d5bb0c15778c0107e55629959b2a9))
* **database:** unified database ([e32e3bb](https://github.com/tr1xem/AWCC/commit/e32e3bb3d8909d733c3ecf235678741eb1bfc0e2))
* **device-info:** Imporve device info detection logic ([af365bc](https://github.com/tr1xem/AWCC/commit/af365bce5edf9866eea2f4be148692493c826231))
* **device-info:** Imporve device info detection logic ([848110f](https://github.com/tr1xem/AWCC/commit/848110f833f40dac47c1a3a6cc426703f2ab47f5))
* **gui:** build versions from macro ([95734f6](https://github.com/tr1xem/AWCC/commit/95734f67ba88a510e0cbc51450261d1841e15c27))
* **gui:** gui tweaks ([67b8586](https://github.com/tr1xem/AWCC/commit/67b8586f2d1f6b601fff8c77541b37d6cbad7554))
* **lights:** better way ([d9992db](https://github.com/tr1xem/AWCC/commit/d9992dbf37a67bcccfd59861da2bba5a89ab3c65))
* **loguru:** hacky verbosity set ([935beee](https://github.com/tr1xem/AWCC/commit/935beee645e4cbfb85132eb1e262393ca31f8564))
* **readme:** licence + cleanup ([9973f54](https://github.com/tr1xem/AWCC/commit/9973f54932755b2ffb5f8d487c5da71f73176701))
* **thermal:** cool mode for testing ([59a31e0](https://github.com/tr1xem/AWCC/commit/59a31e0824faf3c25f2b583c9d6e8ac7766cab12))
* **thermals:** fix log order ([13a29f1](https://github.com/tr1xem/AWCC/commit/13a29f1138fa0da9b705d93d87e46386641fdd83))
* **ui:** tweaks and added font ([90f9b38](https://github.com/tr1xem/AWCC/commit/90f9b3857b82ee7f227fbc0a837f448efd64004b))
* **workflow:** workflow fix ([597addf](https://github.com/tr1xem/AWCC/commit/597addfb3be7032aae54b22a218541c32ac9667e))

## [1.1.0](https://github.com/tr1xem/AWCC/compare/v1.0.0...v1.1.0) (2025-10-01)


### 🚀 New Features

* **acpi:** exit instead of crash ([dab685f](https://github.com/tr1xem/AWCC/commit/dab685ff0bc2aa70e8fad4adc10a135cd41abe9e))
* **acpi:** Fallback deviceinfo for adding new mode ([a4a196c](https://github.com/tr1xem/AWCC/commit/a4a196cd566eaf5c01ce0611e581f58abfbb7622))
* **autorealse:** testing ([f6d74c0](https://github.com/tr1xem/AWCC/commit/f6d74c0866d79ddd44b55b2e58c9675a8cf5a1d8))
* **awcc:** follow standard c++ project structure ([42b66a5](https://github.com/tr1xem/AWCC/commit/42b66a5bee8ef579e8fd7cc028cfe654fe13b5e8))
* **awcc:** use new device detection API and merge AWCC interface ([72d41d3](https://github.com/tr1xem/AWCC/commit/72d41d38b38909e9e550998e61e161bc4cc41f38))
* **clangtidy:** add clang-tidy ([875e215](https://github.com/tr1xem/AWCC/commit/875e215aff7c2d0c54de942dec12b6b92ab141ed))
* **cli:** added full cli mode support ([c9eb07e](https://github.com/tr1xem/AWCC/commit/c9eb07e2cde7e75480a312dfaff756fffb81f8fc))
* **cli:** turbo ([8936a19](https://github.com/tr1xem/AWCC/commit/8936a19c81856a84a3b810f725b03d9a4f30c36d))
* **daemon:** add daemon support ([caff99f](https://github.com/tr1xem/AWCC/commit/caff99f93782d00aa805a22be0fbddd5476bbfaf))
* **daemon:** sanatize commands first ([939022b](https://github.com/tr1xem/AWCC/commit/939022b4657a65523cc11999e05272d624bd50a6))
* **device_detection:** add support for multiple devices ([0b617af](https://github.com/tr1xem/AWCC/commit/0b617aff2cd26689a73164acafac9d6238875702))
* **device_resolution:** Initialize ACPI in correct order and Handel ([4b2bc61](https://github.com/tr1xem/AWCC/commit/4b2bc61aec1657717cf594846c41e42730e08f09))
* **effectsfx:** added all effects and keybinds ([0e2005b](https://github.com/tr1xem/AWCC/commit/0e2005ba638712cf50c4ad2bcf4d9fdc775aa87c))
* **gkey:** gkey support ([11151d8](https://github.com/tr1xem/AWCC/commit/11151d883468a9df22154b4ecd548ae1fdf5167e))
* **gmode:** added Gmode flag ([76f1fb4](https://github.com/tr1xem/AWCC/commit/76f1fb4cfd757050a4a815eae41bb958e14a130a))
* **gui:** fallback fonts and remove print ([d1bf5dc](https://github.com/tr1xem/AWCC/commit/d1bf5dcf03356b21a72e682d1ea972f1aa43bf41))
* **GUI:** Final GUI Push ([3b8569d](https://github.com/tr1xem/AWCC/commit/3b8569d693ee94bc320a2692224e88eeb95cfffe))
* **gui:** GUI with initial features ([b54371e](https://github.com/tr1xem/AWCC/commit/b54371ece0635ff7724a0e4f85bcc8a4375e341f))
* **hacking:** new commands ([6727544](https://github.com/tr1xem/AWCC/commit/67275441527dff1ca6cd957b8e125374fbccfcdd))
* **install:** cmake install script ([5b10bf9](https://github.com/tr1xem/AWCC/commit/5b10bf99c221afef3b0d0193cca7a4178a4b18be))
* **install:** install script for testers ([bc2fab6](https://github.com/tr1xem/AWCC/commit/bc2fab6fe3c1daa5ba4971b9d8b3c10b2df5d944))
* **keybinder:** add keybinder module ([27f3c91](https://github.com/tr1xem/AWCC/commit/27f3c9118a86380a111d2f361384869a3a629dd3))
* **lighfx:** added lightfx module ([59a35a7](https://github.com/tr1xem/AWCC/commit/59a35a74952a69ab9d82a2df9a816a3fa0247e61))
* **release:** config ([771a553](https://github.com/tr1xem/AWCC/commit/771a5538c7c75fc9497423d97851df4485468b44))
* **sockets:** intial sockets ([48a960f](https://github.com/tr1xem/AWCC/commit/48a960f580a7a4e24223168b8d526a4a71ef9e96))
* **superboost:** basic merge of superboost ([541fcc0](https://github.com/tr1xem/AWCC/commit/541fcc01167271e2c50ced24f4df25fbb1b670a3))
* **thermal:** add support for cpu check ([a4aa49c](https://github.com/tr1xem/AWCC/commit/a4aa49c50026564fdada0d1cb044de6da4ba9896))
* **thermal:** querymode and toggleGmode ([f8f7d81](https://github.com/tr1xem/AWCC/commit/f8f7d8194aec9cf71cc23775d2e66713f545d9be))
* **thermals:** new methods ([f883ed6](https://github.com/tr1xem/AWCC/commit/f883ed6ff3713ff16996356de1649854afbe3a4c))
* **workflow:** realease workflow ([fa0ab26](https://github.com/tr1xem/AWCC/commit/fa0ab26d95c3f67a5484057ad3697ed1a8792767))
* **workflow:** realease workflow ([d8fe7bf](https://github.com/tr1xem/AWCC/commit/d8fe7bf2adeb5370dd6201bd9fb208b347446107))


### 🐛 Bug Fixes

* **acpi detection:** removed acip_call utility ([0f2faa0](https://github.com/tr1xem/AWCC/commit/0f2faa00c39c4a83127795b0c6f49ae7fa530a5f))
* **acpi:** return hex value ([8530b9f](https://github.com/tr1xem/AWCC/commit/8530b9fd9788e297c880bbcc45d5cc16f7214169))
* **daemon:** already in mode added ([b349976](https://github.com/tr1xem/AWCC/commit/b34997673c2f546f3ebfb7212ed0d643b68c0628))
* **daemon:** already in mode added ([b01d00b](https://github.com/tr1xem/AWCC/commit/b01d00bf654ee3cca5e88a59f737fad43775b89e))
* **daemon:** remove device detection logic ([831672d](https://github.com/tr1xem/AWCC/commit/831672d7e61314bd35cd38d9ad19c5d6afeeccbe))
* **daemon:** remove device detection logic ([d3665f4](https://github.com/tr1xem/AWCC/commit/d3665f4a830eed2ab139031bf3c24b09e7298d3f))
* **device-info:** sudo fallback for acip ([5e1416a](https://github.com/tr1xem/AWCC/commit/5e1416aff21231b90e90b6f9a8c49527d9d6e7d8))
* **device-info:** sudo fallback for acip ([c621a24](https://github.com/tr1xem/AWCC/commit/c621a24dcba8b9ebccfbc6154c985df586f78af0))
* **gmode:** fix gmode switching and toggle ([6fe5a08](https://github.com/tr1xem/AWCC/commit/6fe5a08eed34d7c892142b7ac235882cb3a8c95d))
* **light:** always send 33-byte zero-padded packets ([7fc57e5](https://github.com/tr1xem/AWCC/commit/7fc57e561734e54c1861c62dc59271cd1e506f71))
* **light:** strip to 33 byte & imgui enhance ([635ea0a](https://github.com/tr1xem/AWCC/commit/635ea0a8f21c7336ab6b8fd0c8fe816ee13ca3f9))
* **light:** use dmi and usb detection ([b2b31fc](https://github.com/tr1xem/AWCC/commit/b2b31fc02546ccbe4a3500d2449e096f037ef41f))


### ⚙️ Chores

* **main:** release 1.1.0 ([a475fd4](https://github.com/tr1xem/AWCC/commit/a475fd4c30852535f22200d6907263b67d754085))


### ♻️ Code Refactoring

* **acpi:** print acpicode & g16 7630 support ([f07fff4](https://github.com/tr1xem/AWCC/commit/f07fff46b6bb0ce59a155115d3897b32a3b59302))
* **acpi:** remove the need of acpiModelID ([4262bb0](https://github.com/tr1xem/AWCC/commit/4262bb0f5bd825c1f3ef069220549edad6a60df2))
* **acpiutils:** Instanciate acpiutil only once ([a3e7edf](https://github.com/tr1xem/AWCC/commit/a3e7edf9d4e11b696c069bcd89c1ced8bb06ce8e))
* **awcc:** structs ([74d3811](https://github.com/tr1xem/AWCC/commit/74d3811f6c20819b592874912951d452da59c6b9))
* **daemon:** use callbacks & gmode key ([b3c88b2](https://github.com/tr1xem/AWCC/commit/b3c88b2034434624ae306ef1db83e5d496179c49))
* **database:** enum class instead of enum ([23e33a9](https://github.com/tr1xem/AWCC/commit/23e33a98926d5bb0c15778c0107e55629959b2a9))
* **database:** unified database ([e32e3bb](https://github.com/tr1xem/AWCC/commit/e32e3bb3d8909d733c3ecf235678741eb1bfc0e2))
* **device-info:** Imporve device info detection logic ([af365bc](https://github.com/tr1xem/AWCC/commit/af365bce5edf9866eea2f4be148692493c826231))
* **device-info:** Imporve device info detection logic ([848110f](https://github.com/tr1xem/AWCC/commit/848110f833f40dac47c1a3a6cc426703f2ab47f5))
* **gui:** build versions from macro ([95734f6](https://github.com/tr1xem/AWCC/commit/95734f67ba88a510e0cbc51450261d1841e15c27))
* **gui:** gui tweaks ([67b8586](https://github.com/tr1xem/AWCC/commit/67b8586f2d1f6b601fff8c77541b37d6cbad7554))
* **lights:** better way ([d9992db](https://github.com/tr1xem/AWCC/commit/d9992dbf37a67bcccfd59861da2bba5a89ab3c65))
* **loguru:** hacky verbosity set ([935beee](https://github.com/tr1xem/AWCC/commit/935beee645e4cbfb85132eb1e262393ca31f8564))
* **readme:** licence + cleanup ([9973f54](https://github.com/tr1xem/AWCC/commit/9973f54932755b2ffb5f8d487c5da71f73176701))
* **thermal:** cool mode for testing ([59a31e0](https://github.com/tr1xem/AWCC/commit/59a31e0824faf3c25f2b583c9d6e8ac7766cab12))
* **thermals:** fix log order ([13a29f1](https://github.com/tr1xem/AWCC/commit/13a29f1138fa0da9b705d93d87e46386641fdd83))
* **ui:** tweaks and added font ([90f9b38](https://github.com/tr1xem/AWCC/commit/90f9b3857b82ee7f227fbc0a837f448efd64004b))
* **workflow:** workflow fix ([597addf](https://github.com/tr1xem/AWCC/commit/597addfb3be7032aae54b22a218541c32ac9667e))

## [1.1.0](https://github.com/tr1xem/AWCC/compare/v1.0.4...v1.1.0) (2025-10-01)


### 🚀 New Features

* **acpi:** exit instead of crash ([dab685f](https://github.com/tr1xem/AWCC/commit/dab685ff0bc2aa70e8fad4adc10a135cd41abe9e))
* **acpi:** Fallback deviceinfo for adding new mode ([a4a196c](https://github.com/tr1xem/AWCC/commit/a4a196cd566eaf5c01ce0611e581f58abfbb7622))
* **awcc:** follow standard c++ project structure ([42b66a5](https://github.com/tr1xem/AWCC/commit/42b66a5bee8ef579e8fd7cc028cfe654fe13b5e8))
* **awcc:** use new device detection API and merge AWCC interface ([72d41d3](https://github.com/tr1xem/AWCC/commit/72d41d38b38909e9e550998e61e161bc4cc41f38))
* **clangtidy:** add clang-tidy ([875e215](https://github.com/tr1xem/AWCC/commit/875e215aff7c2d0c54de942dec12b6b92ab141ed))
* **cli:** added full cli mode support ([c9eb07e](https://github.com/tr1xem/AWCC/commit/c9eb07e2cde7e75480a312dfaff756fffb81f8fc))
* **cli:** turbo ([8936a19](https://github.com/tr1xem/AWCC/commit/8936a19c81856a84a3b810f725b03d9a4f30c36d))
* **daemon:** add daemon support ([caff99f](https://github.com/tr1xem/AWCC/commit/caff99f93782d00aa805a22be0fbddd5476bbfaf))
* **daemon:** sanatize commands first ([939022b](https://github.com/tr1xem/AWCC/commit/939022b4657a65523cc11999e05272d624bd50a6))
* **device_detection:** add support for multiple devices ([0b617af](https://github.com/tr1xem/AWCC/commit/0b617aff2cd26689a73164acafac9d6238875702))
* **device_resolution:** Initialize ACPI in correct order and Handel ([4b2bc61](https://github.com/tr1xem/AWCC/commit/4b2bc61aec1657717cf594846c41e42730e08f09))
* **effectsfx:** added all effects and keybinds ([0e2005b](https://github.com/tr1xem/AWCC/commit/0e2005ba638712cf50c4ad2bcf4d9fdc775aa87c))
* **gkey:** gkey support ([11151d8](https://github.com/tr1xem/AWCC/commit/11151d883468a9df22154b4ecd548ae1fdf5167e))
* **gmode:** added Gmode flag ([76f1fb4](https://github.com/tr1xem/AWCC/commit/76f1fb4cfd757050a4a815eae41bb958e14a130a))
* **gui:** fallback fonts and remove print ([d1bf5dc](https://github.com/tr1xem/AWCC/commit/d1bf5dcf03356b21a72e682d1ea972f1aa43bf41))
* **GUI:** Final GUI Push ([3b8569d](https://github.com/tr1xem/AWCC/commit/3b8569d693ee94bc320a2692224e88eeb95cfffe))
* **gui:** GUI with initial features ([b54371e](https://github.com/tr1xem/AWCC/commit/b54371ece0635ff7724a0e4f85bcc8a4375e341f))
* **hacking:** new commands ([6727544](https://github.com/tr1xem/AWCC/commit/67275441527dff1ca6cd957b8e125374fbccfcdd))
* **install:** cmake install script ([5b10bf9](https://github.com/tr1xem/AWCC/commit/5b10bf99c221afef3b0d0193cca7a4178a4b18be))
* **install:** install script for testers ([bc2fab6](https://github.com/tr1xem/AWCC/commit/bc2fab6fe3c1daa5ba4971b9d8b3c10b2df5d944))
* **keybinder:** add keybinder module ([27f3c91](https://github.com/tr1xem/AWCC/commit/27f3c9118a86380a111d2f361384869a3a629dd3))
* **lighfx:** added lightfx module ([59a35a7](https://github.com/tr1xem/AWCC/commit/59a35a74952a69ab9d82a2df9a816a3fa0247e61))
* **release:** config ([771a553](https://github.com/tr1xem/AWCC/commit/771a5538c7c75fc9497423d97851df4485468b44))
* **sockets:** intial sockets ([48a960f](https://github.com/tr1xem/AWCC/commit/48a960f580a7a4e24223168b8d526a4a71ef9e96))
* **superboost:** basic merge of superboost ([541fcc0](https://github.com/tr1xem/AWCC/commit/541fcc01167271e2c50ced24f4df25fbb1b670a3))
* **thermal:** add support for cpu check ([a4aa49c](https://github.com/tr1xem/AWCC/commit/a4aa49c50026564fdada0d1cb044de6da4ba9896))
* **thermal:** querymode and toggleGmode ([f8f7d81](https://github.com/tr1xem/AWCC/commit/f8f7d8194aec9cf71cc23775d2e66713f545d9be))
* **thermals:** new methods ([f883ed6](https://github.com/tr1xem/AWCC/commit/f883ed6ff3713ff16996356de1649854afbe3a4c))
* **workflow:** realease workflow ([fa0ab26](https://github.com/tr1xem/AWCC/commit/fa0ab26d95c3f67a5484057ad3697ed1a8792767))
* **workflow:** realease workflow ([d8fe7bf](https://github.com/tr1xem/AWCC/commit/d8fe7bf2adeb5370dd6201bd9fb208b347446107))


### 🐛 Bug Fixes

* **acpi detection:** removed acip_call utility ([0f2faa0](https://github.com/tr1xem/AWCC/commit/0f2faa00c39c4a83127795b0c6f49ae7fa530a5f))
* **acpi:** return hex value ([8530b9f](https://github.com/tr1xem/AWCC/commit/8530b9fd9788e297c880bbcc45d5cc16f7214169))
* **daemon:** already in mode added ([b349976](https://github.com/tr1xem/AWCC/commit/b34997673c2f546f3ebfb7212ed0d643b68c0628))
* **daemon:** already in mode added ([b01d00b](https://github.com/tr1xem/AWCC/commit/b01d00bf654ee3cca5e88a59f737fad43775b89e))
* **daemon:** remove device detection logic ([831672d](https://github.com/tr1xem/AWCC/commit/831672d7e61314bd35cd38d9ad19c5d6afeeccbe))
* **daemon:** remove device detection logic ([d3665f4](https://github.com/tr1xem/AWCC/commit/d3665f4a830eed2ab139031bf3c24b09e7298d3f))
* **device-info:** sudo fallback for acip ([5e1416a](https://github.com/tr1xem/AWCC/commit/5e1416aff21231b90e90b6f9a8c49527d9d6e7d8))
* **device-info:** sudo fallback for acip ([c621a24](https://github.com/tr1xem/AWCC/commit/c621a24dcba8b9ebccfbc6154c985df586f78af0))
* **gmode:** fix gmode switching and toggle ([6fe5a08](https://github.com/tr1xem/AWCC/commit/6fe5a08eed34d7c892142b7ac235882cb3a8c95d))
* **light:** always send 33-byte zero-padded packets ([7fc57e5](https://github.com/tr1xem/AWCC/commit/7fc57e561734e54c1861c62dc59271cd1e506f71))
* **light:** strip to 33 byte & imgui enhance ([635ea0a](https://github.com/tr1xem/AWCC/commit/635ea0a8f21c7336ab6b8fd0c8fe816ee13ca3f9))
* **light:** use dmi and usb detection ([b2b31fc](https://github.com/tr1xem/AWCC/commit/b2b31fc02546ccbe4a3500d2449e096f037ef41f))


### ♻️ Code Refactoring

* **acpi:** print acpicode & g16 7630 support ([f07fff4](https://github.com/tr1xem/AWCC/commit/f07fff46b6bb0ce59a155115d3897b32a3b59302))
* **acpi:** remove the need of acpiModelID ([4262bb0](https://github.com/tr1xem/AWCC/commit/4262bb0f5bd825c1f3ef069220549edad6a60df2))
* **acpiutils:** Instanciate acpiutil only once ([a3e7edf](https://github.com/tr1xem/AWCC/commit/a3e7edf9d4e11b696c069bcd89c1ced8bb06ce8e))
* **awcc:** structs ([74d3811](https://github.com/tr1xem/AWCC/commit/74d3811f6c20819b592874912951d452da59c6b9))
* **daemon:** use callbacks & gmode key ([b3c88b2](https://github.com/tr1xem/AWCC/commit/b3c88b2034434624ae306ef1db83e5d496179c49))
* **database:** enum class instead of enum ([23e33a9](https://github.com/tr1xem/AWCC/commit/23e33a98926d5bb0c15778c0107e55629959b2a9))
* **database:** unified database ([e32e3bb](https://github.com/tr1xem/AWCC/commit/e32e3bb3d8909d733c3ecf235678741eb1bfc0e2))
* **device-info:** Imporve device info detection logic ([af365bc](https://github.com/tr1xem/AWCC/commit/af365bce5edf9866eea2f4be148692493c826231))
* **device-info:** Imporve device info detection logic ([848110f](https://github.com/tr1xem/AWCC/commit/848110f833f40dac47c1a3a6cc426703f2ab47f5))
* **gui:** build versions from macro ([95734f6](https://github.com/tr1xem/AWCC/commit/95734f67ba88a510e0cbc51450261d1841e15c27))
* **gui:** gui tweaks ([67b8586](https://github.com/tr1xem/AWCC/commit/67b8586f2d1f6b601fff8c77541b37d6cbad7554))
* **lights:** better way ([d9992db](https://github.com/tr1xem/AWCC/commit/d9992dbf37a67bcccfd59861da2bba5a89ab3c65))
* **loguru:** hacky verbosity set ([935beee](https://github.com/tr1xem/AWCC/commit/935beee645e4cbfb85132eb1e262393ca31f8564))
* **readme:** licence + cleanup ([9973f54](https://github.com/tr1xem/AWCC/commit/9973f54932755b2ffb5f8d487c5da71f73176701))
* **thermal:** cool mode for testing ([59a31e0](https://github.com/tr1xem/AWCC/commit/59a31e0824faf3c25f2b583c9d6e8ac7766cab12))
* **thermals:** fix log order ([13a29f1](https://github.com/tr1xem/AWCC/commit/13a29f1138fa0da9b705d93d87e46386641fdd83))
* **ui:** tweaks and added font ([90f9b38](https://github.com/tr1xem/AWCC/commit/90f9b3857b82ee7f227fbc0a837f448efd64004b))
* **workflow:** workflow fix ([597addf](https://github.com/tr1xem/AWCC/commit/597addfb3be7032aae54b22a218541c32ac9667e))
