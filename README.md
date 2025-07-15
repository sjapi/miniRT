# miniRT (work in progress)

> A ray tracer project developed at [42](https://42.fr) by [azolotar](https://github.com/sjapi) and [haaghaja](https://github.com/106c13)

**miniRT** is a ray tracer for the 42 curriculum. It renders basic shapes with lighting using clean C and the **minilibX** library.

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/readme_example.png" width="800" />
</p>

---

## Shapes

- Sphere
- Plane
- Cylinder
- Cone

## Features

- Diffuse shading
- Specular highlights
- Skybox

## Controls

- `C` — Enter CAMERA MODE. If already in CAMERA MODE, switch back to RENDER MODE.
  - `W` / `S` — Move forward / backward
  - `A` / `D` — Move left / right
  - `Q` / `E` — Move up / down
  - `H` / `L` — Yaw left / right
  - `J` / `K` — Pitch up / down
  - `+` / `-` — FOV increase / decrease

- `R` — Enter RENDER MODE. If in OBJECT MODE, deselects the selected object.

- To enter OBJECT MODE click on object with mouse
  - `W` — Move along +Y
  - `S` — Move along -Y
  - `A` — Move along +X
  - `Q` — Move along -X
  - `E` — Move along +Z
  - `D` — Move along -Z
  - `X` — Rotate around X axis
  - `Y` — Rotate around Y axis
  - `Z` — Rotate around Z axis

---

## Build & Run

```bash
make
./miniRT scenes/sp.rt
```
