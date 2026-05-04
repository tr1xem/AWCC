# Changelog

## [1.19.0](https://github.com/tr1xem/AWCC/compare/v1.18.1...v1.19.0) (2026-05-04)


### 🚀 New Features

* **device:** add support for Alienware m15 R7 AMD ([077fee7](https://github.com/tr1xem/AWCC/commit/077fee72698ff3f166a5bacd43b6aa2f00f3048f))

## [1.18.1](https://github.com/tr1xem/AWCC/compare/v1.18.0...v1.18.1) (2026-04-25)


### 🐛 Bug Fixes

* **imports:** Replace universal imports with build ([152adef](https://github.com/tr1xem/AWCC/commit/152adef1f999210799b98f07864f596a62f4532a))

## [1.18.0](https://github.com/tr1xem/AWCC/compare/v1.17.0...v1.18.0) (2026-04-25)


### 🚀 New Features

* add pre-start socket cleanup to awccd service and refactor CMake install paths to use variables. ([3f503d6](https://github.com/tr1xem/AWCC/commit/3f503d6ca7f149d149a94f7db0b2a451f4ba4e57))
* **core:** implement dynamic keyboard lighting zones ([6be4bdb](https://github.com/tr1xem/AWCC/commit/6be4bdbc34e62a7bc6c43b9a8b4f1cc5f6f79964))
* **daemon:** Persist brightness across restarts ([72e01af](https://github.com/tr1xem/AWCC/commit/72e01afde9e09bab33a9ba25c1a98be214a7bcb2))
* **dev:** add support for Alienware m15 ([c62b2a1](https://github.com/tr1xem/AWCC/commit/c62b2a1b1a3d5735e6b96578b2fcbe6dc8078b24))
* **device:** Add Alienware Area-51 AAT2250 ([aa8874e](https://github.com/tr1xem/AWCC/commit/aa8874e9fa6e0b63d7f50e1b150758b56e1dd176))
* **device:** add Alienware m15 R7 AMD ([c5132e1](https://github.com/tr1xem/AWCC/commit/c5132e109283c3ee343f6c90773556658262e7a9))
* **device:** add support for Alienware Aurora Ryzen Edition R14 ([94e265d](https://github.com/tr1xem/AWCC/commit/94e265d0857a9b5b29cca4fb31ba1522cade0319))
* **openrc:** add openrc service and build flags ([c5bb03e](https://github.com/tr1xem/AWCC/commit/c5bb03e71550c975933f0618280cb4c675a0a653))


### 🐛 Bug Fixes

* **database:** Json format ([9f8fa44](https://github.com/tr1xem/AWCC/commit/9f8fa445edb3d361af6a4075e25580ec5ec90b5f))
* **device:** add Alienware m15 ([4221c3e](https://github.com/tr1xem/AWCC/commit/4221c3e91ccff8a479f4376e5dde88cfea1ea21d))


### ♻️ Code Refactoring

* **daemon:** no comments ([1d16dfd](https://github.com/tr1xem/AWCC/commit/1d16dfd4820c3e21649d44a973477ce243b315dd))
* **effect:** Clean up lightbar code ([eb243d2](https://github.com/tr1xem/AWCC/commit/eb243d2efe64d3097cc04caba68b8022ef9ed69e))

## [1.17.0](https://github.com/tr1xem/AWCC/compare/v1.16.9...v1.17.0) (2026-03-18)


### 🚀 New Features

* **5525:** add support for 5525 ([b8a9007](https://github.com/tr1xem/AWCC/commit/b8a9007ac3f6a84954c34e743ccd2fda7cfc1d47))
* **acpi:** Add thermal mode testing command ([dcd8f0f](https://github.com/tr1xem/AWCC/commit/dcd8f0fbe93e89515759aafab09edb9a3f4928b1))
* **acpi:** exit instead of crash ([05e6c96](https://github.com/tr1xem/AWCC/commit/05e6c969f0603dd0769090f83538bbf29b21dc06))
* **acpi:** Fallback deviceinfo for adding new mode ([c888cd9](https://github.com/tr1xem/AWCC/commit/c888cd9f4be5a0b3e86c29c3710a67c71fc679da))
* add new device ([855670a](https://github.com/tr1xem/AWCC/commit/855670a24d59d803b9514e97fee4de42f8a3e434))
* add support for Alienware Aurora R15 ([3a1e816](https://github.com/tr1xem/AWCC/commit/3a1e8161c680c838cd726f191e46e021838688a3))
* add support for Alienware Aurora R15 ([ae4aef9](https://github.com/tr1xem/AWCC/commit/ae4aef9fd3508f29411d3a976d826cdae23926d4))
* add support for Alienware Aurora R16 ([4fd2c64](https://github.com/tr1xem/AWCC/commit/4fd2c64f3d0839d5b69e6b09d42994168cb616d1))
* **alienware m16 R2:** all fan modes supported ([0fa740d](https://github.com/tr1xem/AWCC/commit/0fa740d76bd76ede12f42774dba951dd79ec09d5))
* **autorealse:** testing ([fc44032](https://github.com/tr1xem/AWCC/commit/fc440320243abe9cf714f873cf18b3018736ce94))
* **awcc:** follow standard c++ project structure ([b7d4769](https://github.com/tr1xem/AWCC/commit/b7d4769015bdb996f26f85fc51297c6bed6130b5))
* **awcc:** use new device detection API and merge AWCC interface ([6245477](https://github.com/tr1xem/AWCC/commit/6245477a3860a9baba52cc390c75986c250b2f63))
* **clangtidy:** add clang-tidy ([5d3e984](https://github.com/tr1xem/AWCC/commit/5d3e984d4439870edc77ffcf06b717cb978c0146))
* **cli:** added full cli mode support ([e94630f](https://github.com/tr1xem/AWCC/commit/e94630f1a834b48769a3ecaa8b5a99f6f647399c))
* **cli:** turbo ([cabaf10](https://github.com/tr1xem/AWCC/commit/cabaf10630e97318799faecc281705b64b93464e))
* **daemon:** add daemon support ([6ee6f9e](https://github.com/tr1xem/AWCC/commit/6ee6f9e420a7d758883f9d16dc944effb5db6b21))
* **daemon:** sanatize commands first ([d271e07](https://github.com/tr1xem/AWCC/commit/d271e0720c298d18fc8c748b4de6ec3455684968))
* **device_detection:** add support for multiple devices ([2837360](https://github.com/tr1xem/AWCC/commit/28373608194b6f6ced938822bd19bb903ff04455))
* **device_resolution:** Initialize ACPI in correct order and Handel ([2c33aad](https://github.com/tr1xem/AWCC/commit/2c33aadda2bd0377856f79190fb34d7e620634b3))
* **device:** 5510 ([118feea](https://github.com/tr1xem/AWCC/commit/118feeaaf64997a69a13f126633bb29cebb316de))
* **device:** add 16x ([1457ad4](https://github.com/tr1xem/AWCC/commit/1457ad40ee20c41e5912abaed626f4001441754b))
* **device:** Add Alienware 17 R5 ([ca296cf](https://github.com/tr1xem/AWCC/commit/ca296cf9435908882b80edac21046bafe57dce0a))
* **device:** add Alienware 18 Area-51 AA18250 ([a50ca26](https://github.com/tr1xem/AWCC/commit/a50ca26a97c4e81448d784037ca2d2b0e335f161))
* **device:** add Alienware m15 R3 ([a99de9e](https://github.com/tr1xem/AWCC/commit/a99de9ebaed277acfc8fd5ad1f7b32a22ce7dcf1))
* **device:** add Alienware m15 R4 ([dd708bf](https://github.com/tr1xem/AWCC/commit/dd708bf8bce0da9ec14ae5837d793a19087d7161))
* **device:** add Alienware x15 R2 ([647831e](https://github.com/tr1xem/AWCC/commit/647831efd2eb53c672c78734a0b6e7d93e9c2872))
* **device:** add Dell G7 7700 ([30e0dd5](https://github.com/tr1xem/AWCC/commit/30e0dd513e51769e6fb93e8b5573a0078be1cece))
* **device:** add m16 r1 ([d77f039](https://github.com/tr1xem/AWCC/commit/d77f039c8d1b2075c585b701cd189f10e769a211))
* **device:** add support for Alienware Aurora R12 ([d266ee6](https://github.com/tr1xem/AWCC/commit/d266ee6d6cfd258e42dc7559550cf91a8ce4c955))
* **device:** add support for Alienware Aurora R12 ([80e8732](https://github.com/tr1xem/AWCC/commit/80e8732918c1e2299005dfc1a56fbe6c0d6db679))
* **device:** add support for Alienware m18 R1 ([fbeb182](https://github.com/tr1xem/AWCC/commit/fbeb182b8122d06b81e1a6dace2c7e53f690d9fb))
* **device:** add support for Dell G16 7620 ([2bfaf8e](https://github.com/tr1xem/AWCC/commit/2bfaf8e26f7c422c8148a182fbd6d33bfce36c80))
* **device:** area51m ([95aac38](https://github.com/tr1xem/AWCC/commit/95aac389dafdb3c52521ef716dfcda7f76734338))
* **device:** aurora r10 ([2af95c6](https://github.com/tr1xem/AWCC/commit/2af95c6a7c351aead212f771e5d16d5c89e205b0))
* **device:** aurora r10 support ([95efb12](https://github.com/tr1xem/AWCC/commit/95efb129820c7842d1c45c9a1ecaed50bfc9f573))
* **device:** g5 ([69e4f90](https://github.com/tr1xem/AWCC/commit/69e4f90638958c26cf19ec7c9be37ba35914b2b4))
* **device:** m15 Ryz ([2e9f568](https://github.com/tr1xem/AWCC/commit/2e9f568440e5a50da936d60fd3e783e59d184f22))
* **device:** m17 r2 ([c3337f9](https://github.com/tr1xem/AWCC/commit/c3337f9feeb398ba6e2b30b551d9f9134b7a89d2))
* **device:** support Alienware 16 Area-51 AA16250 ([4672485](https://github.com/tr1xem/AWCC/commit/46724854238772dcd6d07db4bc429d680fcfcea9))
* **device:** support Alienware Aurora R9 ([1ea6973](https://github.com/tr1xem/AWCC/commit/1ea69737f330807f89135884241649985d66b041))
* **device:** support Dell G15 Special Edition 5521 ([688d9f5](https://github.com/tr1xem/AWCC/commit/688d9f5bc37c6ce5555590e44a684d0b6221a2dc))
* **device:** support for Alienware 16 Aurora AC16250 ([1e7ca99](https://github.com/tr1xem/AWCC/commit/1e7ca99f68da628049bdb99736ba66e0b2729383))
* **device:** support for Alienware Aurora ACT1250 ([51b3971](https://github.com/tr1xem/AWCC/commit/51b39710a10f98511b678d6a62b8dabd57d1b2b2))
* **device:** support for Alienware m15 R7 ([a247d9e](https://github.com/tr1xem/AWCC/commit/a247d9e5660914f455823f301867041118ab1f06))
* **device:** support for Alienware m17 R5 ([44437ed](https://github.com/tr1xem/AWCC/commit/44437edd73328bff2d59085ce19e0a37e5bc01a1))
* **device:** support for Alienware m18 R1 AMD ([f1d96ab](https://github.com/tr1xem/AWCC/commit/f1d96ab79ff276db134dc70787963ef7db9b271b))
* **device:** support for Alienware x16 R2 ([84be144](https://github.com/tr1xem/AWCC/commit/84be1449f9c60f01c0e3686fc7a25eae922d3af3))
* **device:** support for Dell G15 5515 ([5fbe1d1](https://github.com/tr1xem/AWCC/commit/5fbe1d19e643bafb8d956c0f68caa10c0af876fc))
* **device:** support for G7 7500 ([d8da08c](https://github.com/tr1xem/AWCC/commit/d8da08c99fede8ece925b713b03ad95473eaa0cb))
* **device:** support for m18 r2 ([055df8d](https://github.com/tr1xem/AWCC/commit/055df8d7a78ca8791ceb4fd584db697d8ca71bda))
* **device:** support G3 3590 ([278dcd1](https://github.com/tr1xem/AWCC/commit/278dcd1dcd9041538cfef1940fcf7b3a3d1b89f6))
* **device:** tweak aurora r16 ([dc83c29](https://github.com/tr1xem/AWCC/commit/dc83c29d2320aaab092550656845bd7604504a28))
* **device:** x15 R2 ([55346ce](https://github.com/tr1xem/AWCC/commit/55346ce9a807e0bc6ed23e067cf01c3cb758b3a3))
* **device:** x17 r1 ([0d4b50c](https://github.com/tr1xem/AWCC/commit/0d4b50c47b1e4e059f979978fbdc34bf0a3db82b))
* **effect:** Light Bar Spectrum ([407d930](https://github.com/tr1xem/AWCC/commit/407d9305e2484515778fbe339b65157e06db0b1b))
* **effects:** breathe effect & refactor main.cpp ([2eafef4](https://github.com/tr1xem/AWCC/commit/2eafef43bdb7798f9510a10077937c273532de23))
* **effectsfx:** added all effects and keybinds ([2da7f5f](https://github.com/tr1xem/AWCC/commit/2da7f5f9c024ae7f2ef00f6b7e1523a529a16a4f))
* **gkey:** gkey support ([794525c](https://github.com/tr1xem/AWCC/commit/794525cd0b41dfff7a4fa03b1695798c5737aaa9))
* **gmode:** added Gmode flag ([87e7796](https://github.com/tr1xem/AWCC/commit/87e7796a0e08058a6a26129ca2ea7b7c0accb43a))
* **gmode:** toggle for Dell G15 5515 ([f89b783](https://github.com/tr1xem/AWCC/commit/f89b783143ed12a2e960a2c07361d7695dc61a7f))
* **gui:** fallback fonts and remove print ([c8b0bfb](https://github.com/tr1xem/AWCC/commit/c8b0bfbb663012d59555fb3ffc989ee269e00c39))
* **GUI:** Final GUI Push ([614d3d7](https://github.com/tr1xem/AWCC/commit/614d3d78c82c3480c1ea04e1e040c1d849d62f11))
* **gui:** GUI with initial features ([9056b29](https://github.com/tr1xem/AWCC/commit/9056b295eed03cb884a782293ffabe73e48b6bb7))
* **gui:** static fonts and scaling ([207b55b](https://github.com/tr1xem/AWCC/commit/207b55b00929279e724f78095cafc56a32722785))
* **hacking:** new commands ([f3a943a](https://github.com/tr1xem/AWCC/commit/f3a943a3a28574f9dc328bb6c4a1c296d29e76c1))
* **help:** show app version ([3e45dd0](https://github.com/tr1xem/AWCC/commit/3e45dd0c4062f116b36e79e37ff7250239656c4c))
* **install:** cmake install script ([ab106aa](https://github.com/tr1xem/AWCC/commit/ab106aa6ed443269716b1d96dbdf2c3ea935ebc0))
* **install:** install script for testers ([a092fb7](https://github.com/tr1xem/AWCC/commit/a092fb7ea69671ca9efe02e131a793e18712f2bc))
* **keybinder:** add keybinder module ([85427dd](https://github.com/tr1xem/AWCC/commit/85427dd9b74cc2485faef56ff3126eb177324858))
* **lb:** lightbar brightness toggle ([c17ef9f](https://github.com/tr1xem/AWCC/commit/c17ef9f1eaaabb977a255f72309d7e82c7f7e146))
* **lighfx:** added lightfx module ([9138693](https://github.com/tr1xem/AWCC/commit/91386934cdb51243bf02590ad1bfe0167109a6a4))
* **lightbar:** added lightbar support ([84eff1a](https://github.com/tr1xem/AWCC/commit/84eff1ab8c58e397c23f70e664a80166660238cd))
* **lightbar:** lightbar rainbow ([31e3c37](https://github.com/tr1xem/AWCC/commit/31e3c3785ce98323b2b4b470296c48a254caf031))
* **manual:** add manual mode ([a1b76ef](https://github.com/tr1xem/AWCC/commit/a1b76efdf9e92e873f28b8ff633b6320aae0c3f2))
* **release:** config ([0293e73](https://github.com/tr1xem/AWCC/commit/0293e73904712ea240005b470d487807ac67e15e))
* **sockets:** intial sockets ([39425f0](https://github.com/tr1xem/AWCC/commit/39425f06cc238d5c808b5a252fec1da9076e4484))
* **static:** Build and link statically ([267ff04](https://github.com/tr1xem/AWCC/commit/267ff04068fdf09c3398b2c92571fc9b8cdb349f))
* **superboost:** basic merge of superboost ([e177324](https://github.com/tr1xem/AWCC/commit/e1773247247b2255a5a3e14ca16b98dbf3d1e24a))
* **support:** added support for G15 5520 ([f446b96](https://github.com/tr1xem/AWCC/commit/f446b96c5e4b3c9ad91e64aedaff360922a3a698))
* **testmode:** added testmode flag ([472d9ee](https://github.com/tr1xem/AWCC/commit/472d9ee3bfebafee9ca2c0e6b86e11abe82b64b9))
* **testzone:** tests light zone ([a17eb5c](https://github.com/tr1xem/AWCC/commit/a17eb5c67dd8733864ccca43c0dbfb4b9586f2f1))
* **thermal:** add support for cpu check ([d9e2715](https://github.com/tr1xem/AWCC/commit/d9e2715cd1ed4528186cd56f1a690c934f7aa656))
* **thermal:** querymode and toggleGmode ([a355235](https://github.com/tr1xem/AWCC/commit/a35523557bfc6f03bf16e3703879e76730ea9579))
* **thermals:** new methods ([bd427b1](https://github.com/tr1xem/AWCC/commit/bd427b1352e2d7e122ca26fa9cc4918016c7323d))
* **workflow:** realease workflow ([6b07d44](https://github.com/tr1xem/AWCC/commit/6b07d440c62ab3014a9b3d291f56750805e942c0))
* **workflow:** realease workflow ([a7ce1ad](https://github.com/tr1xem/AWCC/commit/a7ce1adbd336a09e47066523e367ae7c66238f89))
* **workflows:** auto set project version to release tag ([26a2a1b](https://github.com/tr1xem/AWCC/commit/26a2a1bbe35282ad240410303a1294d5da16cb6f))


### 🐛 Bug Fixes

* **5515:** custom tweaks ([8c3b880](https://github.com/tr1xem/AWCC/commit/8c3b8809459321355d10cf8ac2146b239903b0a4))
* **acpi detection:** removed acip_call utility ([d970fbc](https://github.com/tr1xem/AWCC/commit/d970fbcd0bb5068b52cd32945ba49d9fcc10323d))
* **acpi:** return hex value ([9698a36](https://github.com/tr1xem/AWCC/commit/9698a36a2f0ee3a3bab66bc0c53be918426abbec))
* add missing device alienware aurora r7 ([ceb92f9](https://github.com/tr1xem/AWCC/commit/ceb92f922635d3ca22991cd92287212d594fe635))
* add tweaks for Alienware m15 R4 ([1e3a447](https://github.com/tr1xem/AWCC/commit/1e3a44794aa16457ef1189679c5ff1be307db944))
* **alienware:** leagcy ([f4c258f](https://github.com/tr1xem/AWCC/commit/f4c258f3425a34fd38a71c0cc855f46b7a5e23bf))
* **alienware:** support area-51m ([2a5e1d6](https://github.com/tr1xem/AWCC/commit/2a5e1d6fe2e8b1a731eab9496ce3263df54a52bf))
* **area51:** tweaks ([26e15ff](https://github.com/tr1xem/AWCC/commit/26e15ffe173e93667b04ff5d95e0962f40068d9e))
* **build:** fix libevdev build ([08b6cbc](https://github.com/tr1xem/AWCC/commit/08b6cbce0f7ac6372797f17a27ad1bbe7066e1f0))
* **build:** fix libevdev build ([6f19b0f](https://github.com/tr1xem/AWCC/commit/6f19b0f21471261fbb223f4bb285ee9d9ca1b506))
* **cmake:** disable documentation, tests, and tools ([48ac7fa](https://github.com/tr1xem/AWCC/commit/48ac7fad59745df25c56c26e0a59d09c52f53788))
* **cmake:** force release ([232319e](https://github.com/tr1xem/AWCC/commit/232319e6f819247aad5caf3f667476454bc025c6))
* **daemon:** already in mode added ([fe08cbd](https://github.com/tr1xem/AWCC/commit/fe08cbd6fe6e8f92a1312d006433c1991a6a5835))
* **daemon:** already in mode added ([544f076](https://github.com/tr1xem/AWCC/commit/544f076b105d2bfeccf50deaab920a605b8fdf86))
* **daemon:** remove device detection logic ([949c0d7](https://github.com/tr1xem/AWCC/commit/949c0d73a780f18a3146118c7151aa734b470859))
* **daemon:** remove device detection logic ([3b5b084](https://github.com/tr1xem/AWCC/commit/3b5b0847b03d790aef705c2745662d27c1f3ee49))
* **daemon:** run daemon always ([1c4cff4](https://github.com/tr1xem/AWCC/commit/1c4cff4475c8e98d964ca0f39db3430ad7d47c5b))
* **device-info:** sudo fallback for acip ([db7af9b](https://github.com/tr1xem/AWCC/commit/db7af9b66795e887384a23fc59ee073c35e59e55))
* **device-info:** sudo fallback for acip ([bad2c29](https://github.com/tr1xem/AWCC/commit/bad2c29b86c2db165cb3dbbfc1d4b37cc9c44751))
* **device:** x17 prefix ([cddb313](https://github.com/tr1xem/AWCC/commit/cddb3139c861f546cec9ebd3b75660acc8116fe4))
* **elc:** fixed setting default animation ([8fd5c27](https://github.com/tr1xem/AWCC/commit/8fd5c27f8b28f7cd1f9aeeab1fcac06897b49fd4))
* **font:** static footer font ([199815e](https://github.com/tr1xem/AWCC/commit/199815eb90fb8db82f18f4e2195923a3494fb0ec))
* **g3:** tweaks ([fec7be2](https://github.com/tr1xem/AWCC/commit/fec7be2c17cec88df2a84a5606b52053c70114ad))
* **gmode:** fix gmode switching and toggle ([11981d0](https://github.com/tr1xem/AWCC/commit/11981d08c0fecb544dc842aaca363b1fee0ca5d0))
* **gmode:** fix gmode toggle ([7fda74d](https://github.com/tr1xem/AWCC/commit/7fda74d8342fcf318a0ef4c59ae09e1dda7aaa0b))
* **gmode:** For AWCC Ryz edition ([a5879f0](https://github.com/tr1xem/AWCC/commit/a5879f01d14a2082a5a779eee22c81fe369d2c33))
* **gui:** fix gui shinking in hyprland ([be73604](https://github.com/tr1xem/AWCC/commit/be736048da109124667f206ac91b71e50a5ae540))
* **info:** fix device name ([71d934f](https://github.com/tr1xem/AWCC/commit/71d934f5f97d264fcbebbc12cb615cab66c99948))
* **light:** always send 33-byte zero-padded packets ([ccfca71](https://github.com/tr1xem/AWCC/commit/ccfca717520c14f65e64562e4166523cafc126b2))
* **light:** strip to 33 byte & imgui enhance ([16ebf66](https://github.com/tr1xem/AWCC/commit/16ebf661dfe8432475020fccdf0c94a19de50ebc))
* **light:** use dmi and usb detection ([c42b581](https://github.com/tr1xem/AWCC/commit/c42b581fe1467353e3da8f039258e8ccc74b574b))
* **m15r3:** tweaks ([3b25813](https://github.com/tr1xem/AWCC/commit/3b2581315741200eb0c0c3bd62025e71a90e088a))
* **m16:** tweaks ([9d1cafc](https://github.com/tr1xem/AWCC/commit/9d1cafc9a46d988567a220c54a6cccf8ecdc142a))
* **main:** static link libevdev ([d153dfd](https://github.com/tr1xem/AWCC/commit/d153dfd22c7c1619a69956d43b19c1dc9d2760b4))
* **prefix:** for m18 ([2e1978c](https://github.com/tr1xem/AWCC/commit/2e1978c6d053d5752ea80ffd1068413aaf35a1bd))
* **rainbow:** tempo ([e3be4b5](https://github.com/tr1xem/AWCC/commit/e3be4b5ea26aca6b841263bca175ff24fa79aaa6))
* **service:** new cmd line ([0accf68](https://github.com/tr1xem/AWCC/commit/0accf685d3655ab2ed52d52b123325cc801cce8c))
* **workflow:** add udev ([2062d43](https://github.com/tr1xem/AWCC/commit/2062d43fb3234a6146a900f7fd4cc5c02fcbfe0f))
* **workflow:** glfw ([928519c](https://github.com/tr1xem/AWCC/commit/928519cefcc4d7a449f524575f111187eb49bfd1))
* **workflow:** missing dep ([d97532d](https://github.com/tr1xem/AWCC/commit/d97532d744db2d7830d4ab203c73575c6ff7705f))
* **workflows:** gl ([110a98d](https://github.com/tr1xem/AWCC/commit/110a98d55d7ba9f176a414c58687bed854f87ff6))
* **workflow:** wrong deb pkg ([9ea2eb0](https://github.com/tr1xem/AWCC/commit/9ea2eb001236b1db8869715fc59a1fa7f5e2d225))


### ⚙️ Chores

* **device:** device tested ([baab541](https://github.com/tr1xem/AWCC/commit/baab541426d16b7900899522b27900c4b244e1f2))
* **logs:** redundancy ([e12ccca](https://github.com/tr1xem/AWCC/commit/e12cccab5c89b301f0a8fd9005bb2ae0a7e1343d))
* **main:** release 1.1.0 ([84243ab](https://github.com/tr1xem/AWCC/commit/84243ab08d3359fa98cd31e0a6a354f14b4e71b4))
* **main:** release 1.1.0 ([1e3a8f2](https://github.com/tr1xem/AWCC/commit/1e3a8f2a222282c5376c952e627e85695636170b))
* **main:** release 1.1.0 ([79fb537](https://github.com/tr1xem/AWCC/commit/79fb537d04d6b06d7afb7f3df542584f89e73b16))
* **main:** release 1.10.0 ([3987923](https://github.com/tr1xem/AWCC/commit/398792373f0065d692c05e0e30f0e28055fa5184))
* **main:** release 1.11.0 ([8de59a1](https://github.com/tr1xem/AWCC/commit/8de59a1d3561fad76bf74d00b5830d27b0242a25))
* **main:** release 1.12.0 ([452fb81](https://github.com/tr1xem/AWCC/commit/452fb81f6fdb77878e1904b0ff79b4b122b4e6f4))
* **main:** release 1.13.0 ([0884143](https://github.com/tr1xem/AWCC/commit/0884143e5fd8ccf983d8a801dcb009e55327c1d9))
* **main:** release 1.14.0 ([a6b9376](https://github.com/tr1xem/AWCC/commit/a6b9376ef9866bbc63b938e378e627a11d61674c))
* **main:** release 1.14.1 ([d4fb6dd](https://github.com/tr1xem/AWCC/commit/d4fb6dd0d2f5de6dd35d7cdafafb135e77f5663a))
* **main:** release 1.15.0 ([b95a550](https://github.com/tr1xem/AWCC/commit/b95a550e60925a605a514f8f5ce9b44ebd02d350))
* **main:** release 1.16.0 ([e8d05f4](https://github.com/tr1xem/AWCC/commit/e8d05f407ee45c33682311c44ad787f0c544a188))
* **main:** release 1.16.1 ([205f775](https://github.com/tr1xem/AWCC/commit/205f775cc2bbee412afecb58ade032319054bbf6))
* **main:** release 1.16.2 ([ae5712f](https://github.com/tr1xem/AWCC/commit/ae5712f1aaec149a4078d01086dd4c0ccdc83851))
* **main:** release 1.16.3 ([5a3d696](https://github.com/tr1xem/AWCC/commit/5a3d696658c62ff991c07ec395ff33e416cbfd51))
* **main:** release 1.16.4 ([e8a62b4](https://github.com/tr1xem/AWCC/commit/e8a62b43520f0645df65dfe29438c00e91676a8f))
* **main:** release 1.16.5 ([038ccdf](https://github.com/tr1xem/AWCC/commit/038ccdf54e8e73d6a87c8ac5bd466530db5fe9e0))
* **main:** release 1.16.6 ([32aec6d](https://github.com/tr1xem/AWCC/commit/32aec6d4a013cccfb1fe2bf9e5bdf7a200edd18c))
* **main:** release 1.16.7 ([0f4797e](https://github.com/tr1xem/AWCC/commit/0f4797ef7caf463d9c55009dfe2cb39f449ae775))
* **main:** release 1.16.8 ([702072b](https://github.com/tr1xem/AWCC/commit/702072b507178d2f68ef4382c9764f2f1e5ec516))
* **main:** release 1.16.9 ([5756a42](https://github.com/tr1xem/AWCC/commit/5756a429f912ddd34e1b481eb7869bb6ee4e283d))
* **main:** release 1.2.0 ([3a38dc1](https://github.com/tr1xem/AWCC/commit/3a38dc1ee87412c1910e6d1a5db845aab8c65ec0))
* **main:** release 1.2.1 ([80659c7](https://github.com/tr1xem/AWCC/commit/80659c79c68aced4e406fde9896318df199051df))
* **main:** release 1.2.2 ([fb39391](https://github.com/tr1xem/AWCC/commit/fb39391d0dfab15ec9ba29d5b2f8c515f04a9c58))
* **main:** release 1.3.0 ([7e6d748](https://github.com/tr1xem/AWCC/commit/7e6d7483c6a57dece153b4f833fc3bd8ee8d54bb))
* **main:** release 1.4.0 ([78c3c1f](https://github.com/tr1xem/AWCC/commit/78c3c1fc07a1bf17a9e8164f192c0cf771629635))
* **main:** release 1.5.0 ([ac821c1](https://github.com/tr1xem/AWCC/commit/ac821c1e982923e570e62b656447e56d5606a848))
* **main:** release 1.6.0 ([d6810e1](https://github.com/tr1xem/AWCC/commit/d6810e1875a0b2f0796ec62e2ec2925928ab8f97))
* **main:** release 1.7.0 ([35ca3be](https://github.com/tr1xem/AWCC/commit/35ca3beffd625007d1fa27ad4fdb3bf589d0bdc2))
* **main:** release 1.7.0 ([425bf94](https://github.com/tr1xem/AWCC/commit/425bf943f124e875f2bb79a4bba5aa08b76f5a71))
* **main:** release 1.8.0 ([dbd953f](https://github.com/tr1xem/AWCC/commit/dbd953f2545992132eceebd8c77b162c54e8df55))
* **main:** release 1.8.1 ([a3777ea](https://github.com/tr1xem/AWCC/commit/a3777ea853c4647554288594a7c7ed2bd6323cec))
* **main:** release 1.9.0 ([dc17143](https://github.com/tr1xem/AWCC/commit/dc171437ccbd98415b11587930a63a1b36a0da74))
* prefix tweaks ([353b684](https://github.com/tr1xem/AWCC/commit/353b68430be0c005a9f8d524867989776dcd3844))
* **quirks:** g5 5505 ([c1feb9a](https://github.com/tr1xem/AWCC/commit/c1feb9aa9a1625defa80462d834323a17f0e896a))
* **readme:** update ([8b6a8f3](https://github.com/tr1xem/AWCC/commit/8b6a8f385ad05d92171cd1d2d067c912713a2f86))
* **readme:** Update ([cb05cb8](https://github.com/tr1xem/AWCC/commit/cb05cb8da2831fcc85223b2a5ce1e430b46da01d))
* **release:** 1.3.0 ([11638cc](https://github.com/tr1xem/AWCC/commit/11638cc7ca759187301ec7d89ae8a2b57f40eb53))
* **tweak:** add g5 5590 tweaks ([42ebaed](https://github.com/tr1xem/AWCC/commit/42ebaed31ca706dd88a3a8ac07e293391919f511))
* **tweaks:** add support for Alienware m17 R2 ([db0d318](https://github.com/tr1xem/AWCC/commit/db0d318fe59bf6bd645e6d83a08a3296b4579c25))
* **update:** add database to release ([3fd0e2c](https://github.com/tr1xem/AWCC/commit/3fd0e2c49d3b258899a4978c29718a1d1b4de4b5))
* **update:** bump to ver 1.2.0 ([c561c02](https://github.com/tr1xem/AWCC/commit/c561c02807e8374a351e423f0d50c8d84cd79f7f))


### ♻️ Code Refactoring

* **acpi:** AMW3 Tweaks ([af56b3f](https://github.com/tr1xem/AWCC/commit/af56b3f1127b0800dc782414497998f7757448c3))
* **acpi:** print acpicode & g16 7630 support ([8ca7fb1](https://github.com/tr1xem/AWCC/commit/8ca7fb1cd8d8fc5f87fc3579a45466f1601f1c93))
* **acpi:** remove the need of acpiModelID ([10b1f8e](https://github.com/tr1xem/AWCC/commit/10b1f8ede19077de16d2a6908d753027a54cc220))
* **acpi:** thermal mode reference ([fd04ac0](https://github.com/tr1xem/AWCC/commit/fd04ac03b6c8852efcee1f18b237a6a9c3bce2a5))
* **acpiutils:** Instanciate acpiutil only once ([f930c0b](https://github.com/tr1xem/AWCC/commit/f930c0bebcc06134618aff0787b176a5e30c5955))
* **acpi:** x15 tweaks ([b1d0227](https://github.com/tr1xem/AWCC/commit/b1d02277a63ad496f8c1fa026d568fe688bd56d5))
* **awcc:** structs ([8dc6dd8](https://github.com/tr1xem/AWCC/commit/8dc6dd82a58173a0a739fa1fc0b58a3b741a7ff4))
* **build:** Refactor build system ([2564fa7](https://github.com/tr1xem/AWCC/commit/2564fa7fc710fd69a3e246b8dc08d39262b35a59))
* **daemon:** use callbacks & gmode key ([7433146](https://github.com/tr1xem/AWCC/commit/74331460ff64420aaab3c6538952c1c3972e18e4))
* **database:** enum class instead of enum ([c45531c](https://github.com/tr1xem/AWCC/commit/c45531c69107f0515c7f42ea0b7164100124085d))
* **database:** unified database ([ce6eff6](https://github.com/tr1xem/AWCC/commit/ce6eff6a60a52d50b1d6707869877d52fd07e441))
* **device-info:** Imporve device info detection logic ([bc39627](https://github.com/tr1xem/AWCC/commit/bc3962767aa14968ee949c343b260e4aa69839b8))
* **device-info:** Imporve device info detection logic ([e973dd3](https://github.com/tr1xem/AWCC/commit/e973dd32d784f4463022756a6e5c80138f3002fb))
* **effect:** naming convention ([efeb40c](https://github.com/tr1xem/AWCC/commit/efeb40ca293dafdf053a19df391b82541bbd2d8d))
* **gui:** build versions from macro ([3a1572e](https://github.com/tr1xem/AWCC/commit/3a1572e2ef421e132572d93d3b267abe8b84ab48))
* **gui:** gui tweaks ([146776c](https://github.com/tr1xem/AWCC/commit/146776c55e2b66aaa9fb1ae59ffe4c497cd379d1))
* **helper:** minor refactor ([89c5253](https://github.com/tr1xem/AWCC/commit/89c525349370d102be347a040e87b1d6547c5802))
* **lights:** better way ([c025855](https://github.com/tr1xem/AWCC/commit/c025855715d8273041206ad6e8503a1e274e73ec))
* **log:** logging ([e8f7caa](https://github.com/tr1xem/AWCC/commit/e8f7caaf566f243a0991a167fa6fd3ef0f9626db))
* **loguru:** hacky verbosity set ([c5f1746](https://github.com/tr1xem/AWCC/commit/c5f1746ebe28fe7c2030513d46769a06fb77d1e9))
* **main:** test mode optimization ([9d69689](https://github.com/tr1xem/AWCC/commit/9d69689d1aa06752878eee8cbfdc278ae02f9e2c))
* **readme:** licence + cleanup ([e2fedb6](https://github.com/tr1xem/AWCC/commit/e2fedb6061b5c4f0f0d8af34fa8fa64304a58c69))
* **readme:** Removed deps ([b798f4d](https://github.com/tr1xem/AWCC/commit/b798f4d493460f742e25c2906c57955474c48941))
* **tester:** set brighntess to 100 before packet ([c6d45ad](https://github.com/tr1xem/AWCC/commit/c6d45ad9fb9bf95c460cf4cd73e92b431b263073))
* **tests:** add bright color ([54f481a](https://github.com/tr1xem/AWCC/commit/54f481ac01633542b266cad49aa57a5241f42af7))
* **thermal:** bidirectional patching ([8a78c78](https://github.com/tr1xem/AWCC/commit/8a78c785617b377e8b66d8a91b98e68880ec5345))
* **thermal:** cool mode for testing ([1d16b0f](https://github.com/tr1xem/AWCC/commit/1d16b0fb86dbbd4445e076a6895da689830cdfe3))
* **thermals:** fix log order ([7f2cde2](https://github.com/tr1xem/AWCC/commit/7f2cde2c2440e68df9098feb4290b613f19bef82))
* **ui:** tweaks and added font ([6a4913f](https://github.com/tr1xem/AWCC/commit/6a4913f029855d98be8c2ad2826849c55cb96afc))
* **workflow:** Include database.json in release tarball ([08ad8eb](https://github.com/tr1xem/AWCC/commit/08ad8ebf0a28d435e77bc86e3aa9b20bbffc9ae2))
* **workflow:** workflow fix ([913b144](https://github.com/tr1xem/AWCC/commit/913b144dd06771b886a63f138486c9f58eb59a20))

## [1.16.9](https://github.com/tr1xem/AWCC/compare/v1.16.8...v1.16.9) (2026-01-21)


### 🐛 Bug Fixes

* **build:** fix libevdev build ([49744c6](https://github.com/tr1xem/AWCC/commit/49744c630db858a5b8ac4f7753b2a33c10d7cafa))

## [1.16.8](https://github.com/tr1xem/AWCC/compare/v1.16.7...v1.16.8) (2026-01-21)


### 🐛 Bug Fixes

* **main:** static link libevdev ([e7c9a7c](https://github.com/tr1xem/AWCC/commit/e7c9a7c3e1c1d6df54843b188dc715e91bfc33c7))

## [1.16.7](https://github.com/tr1xem/AWCC/compare/v1.16.6...v1.16.7) (2026-01-21)


### 🐛 Bug Fixes

* **build:** fix libevdev build ([80663b6](https://github.com/tr1xem/AWCC/commit/80663b6d522ccf7f9ff484a59a0c46eb9d8e58d1))

## [1.16.6](https://github.com/tr1xem/AWCC/compare/v1.16.5...v1.16.6) (2026-01-21)


### 🐛 Bug Fixes

* **workflow:** add udev ([748371d](https://github.com/tr1xem/AWCC/commit/748371d3918e213227a936aeef7267e497d5a134))

## [1.16.5](https://github.com/tr1xem/AWCC/compare/v1.16.4...v1.16.5) (2026-01-21)


### 🐛 Bug Fixes

* **cmake:** disable documentation, tests, and tools ([36cda5b](https://github.com/tr1xem/AWCC/commit/36cda5bb19d09972fb33224c8a794c8fd18a4c41))

## [1.16.4](https://github.com/tr1xem/AWCC/compare/v1.16.3...v1.16.4) (2026-01-21)


### 🐛 Bug Fixes

* **workflows:** gl ([0a3affc](https://github.com/tr1xem/AWCC/commit/0a3affcf727a469bc02086de886e3504dcb637ad))

## [1.16.3](https://github.com/tr1xem/AWCC/compare/v1.16.2...v1.16.3) (2026-01-21)


### 🐛 Bug Fixes

* **workflow:** glfw ([537f99d](https://github.com/tr1xem/AWCC/commit/537f99d194a042aad83e8da74a29aac1473b3fe2))

## [1.16.2](https://github.com/tr1xem/AWCC/compare/v1.16.1...v1.16.2) (2026-01-21)


### 🐛 Bug Fixes

* **workflow:** wrong deb pkg ([02a8801](https://github.com/tr1xem/AWCC/commit/02a8801e3c106320f138809f114a8ca4f5976c0b))

## [1.16.1](https://github.com/tr1xem/AWCC/compare/v1.16.0...v1.16.1) (2026-01-21)


### 🐛 Bug Fixes

* **workflow:** missing dep ([0d07e5c](https://github.com/tr1xem/AWCC/commit/0d07e5cfb9164aa5d5cf950fe73e4712c37f1be4))

## [1.16.0](https://github.com/tr1xem/AWCC/compare/v1.15.0...v1.16.0) (2026-01-21)


### 🚀 New Features

* **device:** add 16x ([1296a6c](https://github.com/tr1xem/AWCC/commit/1296a6c098b7b80b6a88cd83446299b0fb97e729))
* **device:** add Alienware m15 R3 ([9014c20](https://github.com/tr1xem/AWCC/commit/9014c2002c61831170ba827dba6dba36fcb9ad2d))
* **device:** add Alienware m15 R4 ([556ade4](https://github.com/tr1xem/AWCC/commit/556ade472c05549ca9a46c907f54d611edd06204))
* **device:** add m16 r1 ([a5229a6](https://github.com/tr1xem/AWCC/commit/a5229a67e3478a0b265795337e6a467540e1320c))
* **device:** add support for Alienware Aurora R12 ([59bc135](https://github.com/tr1xem/AWCC/commit/59bc135f69de037d488d9a5649c1f919a7113df3))
* **device:** add support for Alienware Aurora R12 ([728ad36](https://github.com/tr1xem/AWCC/commit/728ad3675401b6ab91a80366e8112f515bd0897e))
* **device:** support for Alienware m15 R7 ([df1cc11](https://github.com/tr1xem/AWCC/commit/df1cc11e64a19917882c49d2efcbad3bbb024aa4))
* **device:** support G3 3590 ([a0983f2](https://github.com/tr1xem/AWCC/commit/a0983f2941f350a65b717c86eb69ad2037792dc8))
* **static:** Build and link statically ([a6925ce](https://github.com/tr1xem/AWCC/commit/a6925ce41c7662ef24d48a0a3b89b85e1a133a1d))


### 🐛 Bug Fixes

* add tweaks for Alienware m15 R4 ([b0733e7](https://github.com/tr1xem/AWCC/commit/b0733e72d6f8253d6b1f273434dd4966130e6d0a))
* **g3:** tweaks ([18f40fc](https://github.com/tr1xem/AWCC/commit/18f40fc11f520eece5c7b618720658b76ed28396))
* **m15r3:** tweaks ([a209ebd](https://github.com/tr1xem/AWCC/commit/a209ebd7fd0cd357331ca417ff42c96241a51920))
* **m16:** tweaks ([0a30880](https://github.com/tr1xem/AWCC/commit/0a30880c664ac1003e212fb852c52a8b71715a32))


### ♻️ Code Refactoring

* **build:** Refactor build system ([7987aa0](https://github.com/tr1xem/AWCC/commit/7987aa0c539383e4b0cb05c0a2481511e96e785e))
* **readme:** Removed deps ([497fa3f](https://github.com/tr1xem/AWCC/commit/497fa3f8ef3c9a18c353bd52000b7795eb927f5d))

## [1.15.0](https://github.com/tr1xem/AWCC/compare/v1.14.1...v1.15.0) (2025-12-27)


### 🚀 New Features

* **device:** add Dell G7 7700 ([c27edf7](https://github.com/tr1xem/AWCC/commit/c27edf7ae02bf0bf244ab6f24e69b6a3448ff2d1))
* **device:** add support for Alienware m18 R1 ([97d7e8b](https://github.com/tr1xem/AWCC/commit/97d7e8bd7e946ce1b8d38e3d911b2a6f041a73cf))
* **device:** x17 r1 ([c56a89d](https://github.com/tr1xem/AWCC/commit/c56a89d0500feea6555ec8c2f50437c80fd57969))


### 🐛 Bug Fixes

* **alienware:** leagcy ([ef6371b](https://github.com/tr1xem/AWCC/commit/ef6371b8a99a2d2910b9f2e5f7c20b85e11a6f5a))
* **alienware:** support area-51m ([110354b](https://github.com/tr1xem/AWCC/commit/110354bde6fa0746be9410fd0360555dd60c322b))
* **device:** x17 prefix ([f93a6d8](https://github.com/tr1xem/AWCC/commit/f93a6d8b7ffec003bc21ce54baa5f147ed6a2327))
* **elc:** fixed setting default animation ([bdce8a7](https://github.com/tr1xem/AWCC/commit/bdce8a73102db6ab565aabce1b318b8db08d3a18))
* **rainbow:** tempo ([2f711b3](https://github.com/tr1xem/AWCC/commit/2f711b37d3b80bd8872a8412b7b5813ce657bc1a))

## [1.14.1](https://github.com/tr1xem/AWCC/compare/v1.14.0...v1.14.1) (2025-12-10)


### 🐛 Bug Fixes

* **font:** static footer font ([2cdfbf3](https://github.com/tr1xem/AWCC/commit/2cdfbf37da387970b700cc458a1b21dbfe5993d9))

## [1.14.0](https://github.com/tr1xem/AWCC/compare/v1.13.0...v1.14.0) (2025-12-09)


### 🚀 New Features

* **device:** m15 Ryz ([a92b134](https://github.com/tr1xem/AWCC/commit/a92b1340dfe51fc57c66b5921f13b04a4d693c9f))
* **gui:** static fonts and scaling ([f1c4c6a](https://github.com/tr1xem/AWCC/commit/f1c4c6a1bb4fda376f61f503c4d1f0b3cb8ef5bf))

## [1.13.0](https://github.com/tr1xem/AWCC/compare/v1.12.0...v1.13.0) (2025-11-22)


### 🚀 New Features

* **device:** 5510 ([2c7653e](https://github.com/tr1xem/AWCC/commit/2c7653ef555b68f3f04a7c14c7f5575261ab2a63))
* **device:** support Alienware Aurora R9 ([865b372](https://github.com/tr1xem/AWCC/commit/865b372fe5e62211e162e70ccf4da7e538ea3598))


### 🐛 Bug Fixes

* **gui:** fix gui shinking in hyprland ([73452d4](https://github.com/tr1xem/AWCC/commit/73452d474d146814a248e78f14d1b44762877dfc))


### ♻️ Code Refactoring

* **acpi:** thermal mode reference ([37e0f98](https://github.com/tr1xem/AWCC/commit/37e0f9853b6a8cb795fdc7cb9fe08ec093bb973d))
* **thermal:** bidirectional patching ([2b16f20](https://github.com/tr1xem/AWCC/commit/2b16f209e851d1d0d92b3ce90cd0846ea25a5cd1))

## [1.12.0](https://github.com/tr1xem/AWCC/compare/v1.11.0...v1.12.0) (2025-11-13)


### 🚀 New Features

* **device:** add support for Dell G16 7620 ([889cc70](https://github.com/tr1xem/AWCC/commit/889cc70777c1b416c0707092340f1ac52618d791))
* **device:** support for Alienware x16 R2 ([892bf72](https://github.com/tr1xem/AWCC/commit/892bf72d9d71a04e8a3b89a102cda56eeedb293e))
* **device:** support for m18 r2 ([2fd86eb](https://github.com/tr1xem/AWCC/commit/2fd86eb9549fb330934defc4fab63f9e7034668a))


### ♻️ Code Refactoring

* **helper:** minor refactor ([a6545ff](https://github.com/tr1xem/AWCC/commit/a6545ff493ae0d97865ea5881d02e757beb5e77e))

## [1.11.0](https://github.com/tr1xem/AWCC/compare/v1.10.0...v1.11.0) (2025-10-27)


### 🚀 New Features

* **device:** support Alienware 16 Area-51 AA16250 ([c766eb0](https://github.com/tr1xem/AWCC/commit/c766eb0421ca61e974f0ce2a327f4e51e0e80de6))
* **device:** support for Alienware 16 Aurora AC16250 ([c90df84](https://github.com/tr1xem/AWCC/commit/c90df849d00f1b101332bb395bcd52302e5d6ff3))

## [1.10.0](https://github.com/tr1xem/AWCC/compare/v1.9.0...v1.10.0) (2025-10-21)


### 🚀 New Features

* **device:** support for Alienware m18 R1 AMD ([ae163bd](https://github.com/tr1xem/AWCC/commit/ae163bd14ff0196e62adf60246540c4a7a4c35d6))
* **gmode:** toggle for Dell G15 5515 ([4b2c28b](https://github.com/tr1xem/AWCC/commit/4b2c28bdd77b4fab8e899b553f104c33fcb1b1fa))
* **lb:** lightbar brightness toggle ([7b76025](https://github.com/tr1xem/AWCC/commit/7b7602508ea31a9368794c79972b72a4162a38b9))


### 🐛 Bug Fixes

* **prefix:** for m18 ([ae3cae2](https://github.com/tr1xem/AWCC/commit/ae3cae2ecc2eb9b76a4fd7ea63c8cc638e19fbaa))


### ⚙️ Chores

* **device:** device tested ([9f322d4](https://github.com/tr1xem/AWCC/commit/9f322d4fbfb78c47325d92132000e1b1989c72e1))

## [1.9.0](https://github.com/tr1xem/AWCC/compare/v1.8.1...v1.9.0) (2025-10-17)


### 🚀 New Features

* **device:** support for Dell G15 5515 ([3d20c8c](https://github.com/tr1xem/AWCC/commit/3d20c8c47bdd7b8c612b1747b33d4b0032768522))
* **help:** show app version ([8f86a9f](https://github.com/tr1xem/AWCC/commit/8f86a9fbb86cc257328ae6d50d08a743585307cb))
* **manual:** add manual mode ([346d785](https://github.com/tr1xem/AWCC/commit/346d785dceccf2c8e3288bfe0e8a36dc87904ba7))


### 🐛 Bug Fixes

* **daemon:** run daemon always ([c922e41](https://github.com/tr1xem/AWCC/commit/c922e41b0221252534026956c4a293570659452c))

## [1.8.1](https://github.com/tr1xem/AWCC/compare/v1.8.0...v1.8.1) (2025-10-16)


### 🐛 Bug Fixes

* **service:** new cmd line ([9be41c5](https://github.com/tr1xem/AWCC/commit/9be41c5386560b162a468ed42edaba3ed64d860e))

## [1.8.0](https://github.com/tr1xem/AWCC/compare/v1.7.0...v1.8.0) (2025-10-15)


### 🚀 New Features

* **5525:** add support for 5525 ([a7accd7](https://github.com/tr1xem/AWCC/commit/a7accd7756df2f8caab5f379e77b712c0adb955a))
* **acpi:** Add thermal mode testing command ([312f2f2](https://github.com/tr1xem/AWCC/commit/312f2f2b78a477a8e11d068ceff3501373db79c3))
* **acpi:** exit instead of crash ([dab685f](https://github.com/tr1xem/AWCC/commit/dab685ff0bc2aa70e8fad4adc10a135cd41abe9e))
* **acpi:** Fallback deviceinfo for adding new mode ([a4a196c](https://github.com/tr1xem/AWCC/commit/a4a196cd566eaf5c01ce0611e581f58abfbb7622))
* **alienware m16 R2:** all fan modes supported ([a76665a](https://github.com/tr1xem/AWCC/commit/a76665a5215837120b1f656b2125b68d87dc5f61))
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
* **device:** support Dell G15 Special Edition 5521 ([55512ac](https://github.com/tr1xem/AWCC/commit/55512ac13186830ad4d847ec4a260b3bb9fedde1))
* **device:** support for Alienware m17 R5 ([a610d2b](https://github.com/tr1xem/AWCC/commit/a610d2b7b82811923ca7de01c9c9d72030140665))
* **device:** support for G7 7500 ([be6bb36](https://github.com/tr1xem/AWCC/commit/be6bb36d44c3325fe9b944693ccf280a42112652))
* **effect:** Light Bar Spectrum ([19ec096](https://github.com/tr1xem/AWCC/commit/19ec09679fbf941aa5678469d7473c20601bfef1))
* **effects:** breathe effect & refactor main.cpp ([f1ccc46](https://github.com/tr1xem/AWCC/commit/f1ccc460947f05a3f97eec5b1fd2ca51b7655ff3))
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
* **lightbar:** added lightbar support ([fa14536](https://github.com/tr1xem/AWCC/commit/fa14536b5803640e554586dfa3233770b78a62a5))
* **lightbar:** lightbar rainbow ([4ab9668](https://github.com/tr1xem/AWCC/commit/4ab966810e66ae3efedda91a15d43a14a922589a))
* **release:** config ([771a553](https://github.com/tr1xem/AWCC/commit/771a5538c7c75fc9497423d97851df4485468b44))
* **sockets:** intial sockets ([48a960f](https://github.com/tr1xem/AWCC/commit/48a960f580a7a4e24223168b8d526a4a71ef9e96))
* **superboost:** basic merge of superboost ([541fcc0](https://github.com/tr1xem/AWCC/commit/541fcc01167271e2c50ced24f4df25fbb1b670a3))
* **support:** added support for G15 5520 ([3f16f71](https://github.com/tr1xem/AWCC/commit/3f16f71e77b0b6cfecee11fdf905aceff4b9819b))
* **testmode:** added testmode flag ([86bd0c6](https://github.com/tr1xem/AWCC/commit/86bd0c62c3cd9c4364e190f41bfafa0a4c1c7985))
* **testzone:** tests light zone ([97f56ad](https://github.com/tr1xem/AWCC/commit/97f56ad10804c8ddbfc00915463cddf49c03241b))
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

* **logs:** redundancy ([c8a65a6](https://github.com/tr1xem/AWCC/commit/c8a65a60340ba9da1ca23acfa176e10b4bb5f53c))
* **main:** release 1.1.0 ([57369cb](https://github.com/tr1xem/AWCC/commit/57369cb891081eeca368707029937fb9fb6a5824))
* **main:** release 1.1.0 ([8017b60](https://github.com/tr1xem/AWCC/commit/8017b6082766ba114d91e90c3cb4722dda93a965))
* **main:** release 1.1.0 ([a475fd4](https://github.com/tr1xem/AWCC/commit/a475fd4c30852535f22200d6907263b67d754085))
* **main:** release 1.2.0 ([00e6bdc](https://github.com/tr1xem/AWCC/commit/00e6bdcd34ea86eac8b9bae19e86625aa8b1db20))
* **main:** release 1.2.1 ([ef759a0](https://github.com/tr1xem/AWCC/commit/ef759a0ae546d42a2b5595394163bd2645305c9e))
* **main:** release 1.2.2 ([41a7166](https://github.com/tr1xem/AWCC/commit/41a7166ae33162e140f4a3df599c8d6b438e690e))
* **main:** release 1.3.0 ([ea4160a](https://github.com/tr1xem/AWCC/commit/ea4160a48ae0f4750769c4256f6a9e18c8a306de))
* **main:** release 1.4.0 ([a1adade](https://github.com/tr1xem/AWCC/commit/a1adade0b6b9eff9cdb9402c96b12d147808392f))
* **main:** release 1.5.0 ([f94506e](https://github.com/tr1xem/AWCC/commit/f94506e2bdceac4f92b63dfa7824ec76ba2ae936))
* **main:** release 1.6.0 ([655d9d5](https://github.com/tr1xem/AWCC/commit/655d9d5465eadd9618d76b2ff5b0c51630002bb7))
* **main:** release 1.7.0 ([4443e02](https://github.com/tr1xem/AWCC/commit/4443e028f33ff99150a1d070858458fa9d54f173))
* **main:** release 1.7.0 ([8bdbd9b](https://github.com/tr1xem/AWCC/commit/8bdbd9b48c884a81b0b3ddde38a32ca9fce84424))
* **release:** 1.3.0 ([391912e](https://github.com/tr1xem/AWCC/commit/391912e925da3073738614a5b03d8e57be3bdae8))
* **update:** add database to release ([e331394](https://github.com/tr1xem/AWCC/commit/e331394c540af1204f8b6f14131f02ae7f710b10))
* **update:** bump to ver 1.2.0 ([7f56a42](https://github.com/tr1xem/AWCC/commit/7f56a42dd67545cbd1314c15083df345bf152c9c))


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
* **effect:** naming convention ([8229281](https://github.com/tr1xem/AWCC/commit/8229281c3d0042efecebce3fe6edf7cf12a3d7a7))
* **gui:** build versions from macro ([95734f6](https://github.com/tr1xem/AWCC/commit/95734f67ba88a510e0cbc51450261d1841e15c27))
* **gui:** gui tweaks ([67b8586](https://github.com/tr1xem/AWCC/commit/67b8586f2d1f6b601fff8c77541b37d6cbad7554))
* **lights:** better way ([d9992db](https://github.com/tr1xem/AWCC/commit/d9992dbf37a67bcccfd59861da2bba5a89ab3c65))
* **loguru:** hacky verbosity set ([935beee](https://github.com/tr1xem/AWCC/commit/935beee645e4cbfb85132eb1e262393ca31f8564))
* **main:** test mode optimization ([2f74978](https://github.com/tr1xem/AWCC/commit/2f749785d95133eec46317cba3233e3d8eeb3376))
* **readme:** licence + cleanup ([9973f54](https://github.com/tr1xem/AWCC/commit/9973f54932755b2ffb5f8d487c5da71f73176701))
* **tester:** set brighntess to 100 before packet ([7f64319](https://github.com/tr1xem/AWCC/commit/7f6431977733fbc89d87d77b75cf9104d485fa70))
* **tests:** add bright color ([444691d](https://github.com/tr1xem/AWCC/commit/444691de8f67936d6a7785e2fe372a295180c0d9))
* **thermal:** cool mode for testing ([59a31e0](https://github.com/tr1xem/AWCC/commit/59a31e0824faf3c25f2b583c9d6e8ac7766cab12))
* **thermals:** fix log order ([13a29f1](https://github.com/tr1xem/AWCC/commit/13a29f1138fa0da9b705d93d87e46386641fdd83))
* **ui:** tweaks and added font ([90f9b38](https://github.com/tr1xem/AWCC/commit/90f9b3857b82ee7f227fbc0a837f448efd64004b))
* **workflow:** Include database.json in release tarball ([4e58585](https://github.com/tr1xem/AWCC/commit/4e585855cd20993c058130a54fbdc25b63f65dd9))
* **workflow:** workflow fix ([597addf](https://github.com/tr1xem/AWCC/commit/597addfb3be7032aae54b22a218541c32ac9667e))

## [1.7.0](https://github.com/tr1xem/AWCC/compare/v1.6.0...v1.7.0) (2025-10-15)


### 🚀 New Features

* **testmode:** added testmode flag ([86bd0c6](https://github.com/tr1xem/AWCC/commit/86bd0c62c3cd9c4364e190f41bfafa0a4c1c7985))


### ⚙️ Chores

* **main:** release 1.7.0 ([8bdbd9b](https://github.com/tr1xem/AWCC/commit/8bdbd9b48c884a81b0b3ddde38a32ca9fce84424))


### ♻️ Code Refactoring

* **main:** test mode optimization ([2f74978](https://github.com/tr1xem/AWCC/commit/2f749785d95133eec46317cba3233e3d8eeb3376))

## [1.7.0](https://github.com/tr1xem/AWCC/compare/v1.6.0...v1.7.0) (2025-10-15)


### 🚀 New Features

* **testmode:** added testmode flag ([86bd0c6](https://github.com/tr1xem/AWCC/commit/86bd0c62c3cd9c4364e190f41bfafa0a4c1c7985))


### ♻️ Code Refactoring

* **main:** test mode optimization ([2f74978](https://github.com/tr1xem/AWCC/commit/2f749785d95133eec46317cba3233e3d8eeb3376))

## [1.6.0](https://github.com/tr1xem/AWCC/compare/v1.5.0...v1.6.0) (2025-10-14)


### 🚀 New Features

* **acpi:** Add thermal mode testing command ([312f2f2](https://github.com/tr1xem/AWCC/commit/312f2f2b78a477a8e11d068ceff3501373db79c3))
* **device:** support for Alienware m17 R5 ([a610d2b](https://github.com/tr1xem/AWCC/commit/a610d2b7b82811923ca7de01c9c9d72030140665))
* **device:** support for G7 7500 ([be6bb36](https://github.com/tr1xem/AWCC/commit/be6bb36d44c3325fe9b944693ccf280a42112652))
* **effect:** Light Bar Spectrum ([19ec096](https://github.com/tr1xem/AWCC/commit/19ec09679fbf941aa5678469d7473c20601bfef1))
* **effects:** breathe effect & refactor main.cpp ([f1ccc46](https://github.com/tr1xem/AWCC/commit/f1ccc460947f05a3f97eec5b1fd2ca51b7655ff3))
* **lightbar:** added lightbar support ([fa14536](https://github.com/tr1xem/AWCC/commit/fa14536b5803640e554586dfa3233770b78a62a5))
* **lightbar:** lightbar rainbow ([4ab9668](https://github.com/tr1xem/AWCC/commit/4ab966810e66ae3efedda91a15d43a14a922589a))


### ⚙️ Chores

* **logs:** redundancy ([c8a65a6](https://github.com/tr1xem/AWCC/commit/c8a65a60340ba9da1ca23acfa176e10b4bb5f53c))


### ♻️ Code Refactoring

* **effect:** naming convention ([8229281](https://github.com/tr1xem/AWCC/commit/8229281c3d0042efecebce3fe6edf7cf12a3d7a7))

## [1.5.0](https://github.com/tr1xem/AWCC/compare/v1.4.0...v1.5.0) (2025-10-09)


### 🚀 New Features

* **device:** support Dell G15 Special Edition 5521 ([55512ac](https://github.com/tr1xem/AWCC/commit/55512ac13186830ad4d847ec4a260b3bb9fedde1))

## [1.4.0](https://github.com/tr1xem/AWCC/compare/v1.3.0...v1.4.0) (2025-10-07)


### 🚀 New Features

* **5525:** add support for 5525 ([a7accd7](https://github.com/tr1xem/AWCC/commit/a7accd7756df2f8caab5f379e77b712c0adb955a))

## [1.3.0](https://github.com/tr1xem/AWCC/compare/v1.2.2...v1.3.0) (2025-10-04)


### 🚀 New Features

* **support:** added support for G15 5520 ([3f16f71](https://github.com/tr1xem/AWCC/commit/3f16f71e77b0b6cfecee11fdf905aceff4b9819b))


### ⚙️ Chores

* **release:** 1.3.0 ([391912e](https://github.com/tr1xem/AWCC/commit/391912e925da3073738614a5b03d8e57be3bdae8))

## [1.2.2](https://github.com/tr1xem/AWCC/compare/v1.2.1...v1.2.2) (2025-10-04)


### ♻️ Code Refactoring

* **workflow:** Include database.json in release tarball ([4e58585](https://github.com/tr1xem/AWCC/commit/4e585855cd20993c058130a54fbdc25b63f65dd9))

## [1.2.1](https://github.com/tr1xem/AWCC/compare/v1.2.0...v1.2.1) (2025-10-04)


### ⚙️ Chores

* **update:** add database to release ([e331394](https://github.com/tr1xem/AWCC/commit/e331394c540af1204f8b6f14131f02ae7f710b10))
* **update:** bump to ver 1.2.0 ([7f56a42](https://github.com/tr1xem/AWCC/commit/7f56a42dd67545cbd1314c15083df345bf152c9c))

## [1.2.0](https://github.com/tr1xem/AWCC/compare/v1.1.0...v1.2.0) (2025-10-04)


### 🚀 New Features

* **alienware m16 R2:** all fan modes supported ([a76665a](https://github.com/tr1xem/AWCC/commit/a76665a5215837120b1f656b2125b68d87dc5f61))
* **testzone:** tests light zone ([97f56ad](https://github.com/tr1xem/AWCC/commit/97f56ad10804c8ddbfc00915463cddf49c03241b))


### ♻️ Code Refactoring

* **tester:** set brighntess to 100 before packet ([7f64319](https://github.com/tr1xem/AWCC/commit/7f6431977733fbc89d87d77b75cf9104d485fa70))
* **tests:** add bright color ([444691d](https://github.com/tr1xem/AWCC/commit/444691de8f67936d6a7785e2fe372a295180c0d9))

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
