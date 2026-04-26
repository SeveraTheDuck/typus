{
  description = "C++ Template Environment";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = import ./nix/systems.nix;
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; });
    in
    {
      formatter = forAllSystems (system: nixpkgsFor.${system}.nixfmt);

      devShells = forAllSystems (
        system:
        let
          pkgs = nixpkgsFor.${system};
          llvm = pkgs.llvmPackages_latest;
          commonPackages = import ./nix/commonPackages.nix { inherit pkgs llvm; };
          mkCppShell = import ./nix/mkCppShell.nix { inherit pkgs llvm commonPackages; };
        in
        {
          clang = mkCppShell {
            stdenv = llvm.stdenv;
            CC = "${llvm.clang}/bin/clang";
            CXX = "${llvm.clang}/bin/clang++";
            name = "clang";
          };

          gcc = mkCppShell {
            stdenv = pkgs.gcc15Stdenv;
            CC = "${pkgs.gcc15}/bin/gcc";
            CXX = "${pkgs.gcc15}/bin/g++";
            extraPackages = [ pkgs.gcc15 ];
            name = "gcc";
          };

          default = self.devShells.${system}.clang;
        }
      );
    };
}
