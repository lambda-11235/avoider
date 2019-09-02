
with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "devel";
  buildInputs = [
    clang
    ninja meson pkgconfig
    liblapack
    SDL2 SDL2_gfx
  ];
}
