{ stdenv
, lib
, pkgs
, meson
, ninja
, nix-gitignore
, pkg-config
, glib
, libcarbontray
, gtk3
, xorg
}:
stdenv.mkDerivation {
  pname = "carbontray";
  version = "2020-07-11";

  src = nix-gitignore.gitignoreSource [] ./.;

  nativeBuildInputs = [
    meson
    ninja
    pkg-config
  ];

  buildInputs = [
    glib
    gtk3
    libcarbontray
    xorg.libX11
  ];
}
