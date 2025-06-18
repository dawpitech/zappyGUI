{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        cc = pkgs.gcc14;

        deps = with pkgs; [
          libcxx
          gnumake
          criterion
        ] ++ [ cc ];
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            valgrind
            cmake
            doxygen
            pkg-config
            raylib
          ] ++ deps;
        };

        formatter = pkgs.nixpkgs-fmt;
      });
}
