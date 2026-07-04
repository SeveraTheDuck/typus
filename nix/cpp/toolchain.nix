# -- C++ toolchain -------------------------------------------------------------
# Appends C++ tooling to base's append-only `devShellPackages` accumulator. It
# is a list option, so definitions across layers concatenate -- the base default
# shell becomes C++-capable without being redefined.
# perSystem module.

{ pkgs, lib, ... }:
{
  devShellPackages =
    with pkgs;
    [
      cmake
      ninja
      ccache
      clang-tools
      cmake-format
      gcovr
    ]
    ++ lib.optionals stdenv.hostPlatform.isLinux ([ valgrind ]);
}
