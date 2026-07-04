# -- C++ package output --------------------------------------------------------
# Builds the library as an installable Nix package, reusing the project's own
# install(EXPORT) rules: mkDerivation's install phase runs `cmake --install`, so
# the package carries the same Config/Targets/headers/.a a consumer gets from a
# manual install. Tests are off (BUILD_TESTING=OFF) so the sandboxed build needs
# no network for FetchContent GTest. Fortify hardening is dropped because
# ProjectOptions.cmake defines _FORTIFY_SOURCE itself in Release.
# perSystem module.

{ pkgs, lib, ... }:
let
  compilers = import ./compilers.nix { inherit pkgs; };
  toolchain = compilers.gcc;
in
{
  packages.default = toolchain.stdenv.mkDerivation {
    pname = "typus";
    version = lib.strings.trim (builtins.readFile ./../../version.txt);
    src = ./../..;

    nativeBuildInputs = with pkgs; [
      cmake
      ninja
    ];

    hardeningDisable = [ "fortify" ];
    cmakeBuildType = "Release";
    cmakeFlags = [
      (lib.cmakeBool "typus_ENABLE_INSTALL" true)
      (lib.cmakeBool "typus_ENABLE_CCACHE" false)
      (lib.cmakeBool "BUILD_TESTING" false)
      (lib.cmakeBool "CMAKE_CXX_SCAN_FOR_MODULES" false)
    ];
    doCheck = false;
  };
}
