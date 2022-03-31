{
  description = "A very simple standalone tray";

  inputs = {
    flake-utils = {
      url = "github:numtide/flake-utils";
    };

    nixpkgs = {
      url = "github:NixOS/nixpkgs/nixos-unstable";
    };
  };

  outputs = { self, flake-utils, nixpkgs }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages = {
          carbontray = pkgs.callPackage ./carbontray.nix { };
          default = self.packages.${system}.carbontray;
        };
          
      }
    );
}
