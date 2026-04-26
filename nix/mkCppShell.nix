{
  pkgs,
  llvm,
  commonPackages,
}:
{
  stdenv,
  CC,
  CXX,
  extraPackages ? [ ],
  name,
}:

(pkgs.mkShell.override { inherit stdenv; }) {
  packages = commonPackages ++ extraPackages;
  inherit CC CXX;
  # Disabled to avoid conflicts with sanitizers
  hardeningDisable = [ "all" ];

  shellHook = ''
        echo "--- C++ Dev Environment (${name}) ---"
        echo "CC:    $CC ($($CC -dumpversion))"
        echo "CXX:   $CXX ($($CXX -dumpversion))"
        echo "CMake: $(cmake --version | head -n 1 | awk '{print $3}')"
        echo "--------------------------------------"

        if [ -d .git ]; then
          expected='nix develop --command pre-commit run --hook-stage pre-commit'
          if ! grep -qF "$expected" .git/hooks/pre-commit 2>/dev/null; then
            cat > .git/hooks/pre-commit << 'EOF'
    #!/bin/sh
    nix develop --command pre-commit run --hook-stage pre-commit
    EOF
            chmod +x .git/hooks/pre-commit
          fi
        fi
  '';
}
