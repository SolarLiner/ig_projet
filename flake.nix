{
  description = "A very basic flake";

  inputs = {
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = { self, nixpkgs, flake-utils }: with flake-utils.lib; eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
      libPath = with pkgs; lib.makeLibraryPath [
          libGL
          libxkbcommon
          wayland
          xorg.libX11
          xorg.libXcursor
          xorg.libXi
          xorg.libXrandr
        ];
      openmesh = with pkgs; stdenv.mkDerivation rec {
        pname = "openmesh";
        version = "9.0";
        src = builtins.fetchurl {
          url = "https://www.graphics.rwth-aachen.de/media/openmesh_static/Releases/9.0/OpenMesh-9.0.tar.gz";
          sha256 = "sha256:01nhp4nhwc8vs8cr4xgf4v52ifc4b8s7ixcbbbkqqyc22j94qmxr";
        };
        nativeBuildInputs = [ cmake ninja ];
      };
      entt = pkgs.entt.overrideAttrs (_: _: rec {
        version = "3.11.0";
        src = pkgs.fetchFromGitHub {
          owner = "skypjack";
          repo = "entt";
          rev = "v${version}";
          sha256 = "sha256-urB1oU4Riuo9+DKmMf317bqF4hTcm/zsSLn5fpZY27o=";
        };
      });
      ig-projet = with pkgs; stdenv.mkDerivation {
        pname = "ig-projet";
        version = "master";
        src = ./.;
        buildInputs = [ mesa libGL openmesh sfml glm entt ];
        nativeBuildInputs = [ cmake ninja makeWrapper ];
        postInstall = ''
          wrapProgram "$out/bin/ig_projet" --prefix LD_LIBRARY_PATH : "${libPath}"
        '';
      };
    in
    rec {
      packages.openmesh = openmesh;
      packages.ig-projet = ig-projet;
      packages.entt = entt;
      packages.default = packages.ig-projet;

      apps.ig-projet = mkApp { drv = packages.default; };
      apps.default = apps.ig-projet;
      devShell = with pkgs; mkShell {
        buildInputs = [ cmake ninja openmesh entt sfml clang clang-tools mesa libGL ];
        LD_LIBRARY_PATH = libPath;
      };
    }
  );
}
