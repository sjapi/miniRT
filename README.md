# miniRT

> A ray tracer project developed at [42](https://42.fr) by [azolotar](https://github.com/sjapi) and [haaghaja](https://github.com/106c13)

**miniRT** is a ray tracer for the 42 curriculum. It renders basic shapes with lighting using clean C and the **minilibX** library.

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/img/solar.png" width="800" />
</p>

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/img/cols.png" width="800" />
</p>

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/img/rgb_cone.png" width="800" />
</p>

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/img/readme_example.png" width="800" />
</p>

---

## 🔷 Shapes

| Shape | Identifier | Description |
|-------|-----------|-------------|
| **Sphere** | `sp` | Perfect sphere with radius |
| **Plane** | `pl` | Infinite plane with normal vector |
| **Cylinder** | `cy` | Finite cylinder with diameter and height |
| **Cone** | `co` | Cone with apex angle and height |
| **Model** | `obj` | Custom mesh format (triangle-based) |

## 🎨 Textures & Materials

| Feature | Support | Notes |
|---------|---------|-------|
| **Checkerboard** | All shapes | Procedural pattern |
| **Image Textures** | Sphere, Plane, Cylinder, Cone | `.xpm` format |
| **Bump Mapping** | Sphere, Plane, Cylinder, Cone | Normal perturbation from texture |
| **Mirror** | Sphere only | Reflective surfaces |

## ✨ Rendering Features

- **Diffuse Shading** — Lambertian reflection model
- **Specular Highlights** — Phong reflection
- **Multi-sample Anti-aliasing** — 4x MSAA for smooth edges
- **BVH Acceleration** — Bounding Volume Hierarchy for fast ray-object intersection
- **Skybox** — Environment mapping with `.xpm` textures
- **Multiple Lights** — Support for multiple point light sources

## 🎮 Modes

The ray tracer operates in four interactive modes:

| Mode | Description | Entry |
|------|-------------|-------|
| **🖼️ Render Mode** | Default viewing mode | Press `R` or deselect objects |
| **📷 Camera Mode** | Navigate and adjust camera | Press `C` |
| **💡 Light Mode** | Manipulate lighting | Press `L` |
| **🎯 Object Mode** | Edit selected object | Click on any object |

---

## ⌨️ Controls

### 📷 **CAMERA MODE** (Press `C`)

**Movement:**
| Key | Action |
|-----|--------|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Move left |
| `D` | Move right |
| `Q` | Move down |
| `E` | Move up |

**Rotation:**
| Key | Action |
|-----|--------|
| `H` | Yaw right (rotate camera right) |
| `L` | Yaw left (rotate camera left) |
| `J` | Pitch down |
| `K` | Pitch up |

**Field of View:**
| Key | Action |
|-----|--------|
| `+` | Increase FOV |
| `-` | Decrease FOV |

> Press `C` again to return to **RENDER MODE**

---

### 🎯 **OBJECT MODE** (Click on object)

**Translation:**
| Key | Action |
|-----|--------|
| `Q` | Move along +X axis |
| `A` | Move along -X axis |
| `W` | Move along +Y axis |
| `S` | Move along -Y axis |
| `E` | Move along +Z axis |
| `D` | Move along -Z axis |

**Rotation:**
| Key | Action |
|-----|--------|
| `X` | Rotate around X axis |
| `Y` | Rotate around Y axis |
| `Z` | Rotate around Z axis |

**Resize:**
| Key | Object | Action |
|-----|--------|--------|
| `←` / `→` | Sphere | Adjust diameter |
| `←` / `→` | Cylinder | Adjust diameter |
| `←` / `→` | Cone | Adjust apex angle |
| `↑` / `↓` | Cylinder | Adjust height |
| `↑` / `↓` | Cone | Adjust height |

> Press `R` to return to **RENDER MODE**

---

### 💡 **LIGHT MODE** (Press `L`)

**Move All Lights:**
| Key | Action |
|-----|--------|
| `Q` / `A` | Move along X axis |
| `W` / `S` | Move along Y axis |
| `E` / `D` | Move along Z axis |

**Ambient Light:**
| Key | Action |
|-----|--------|
| `O` | Decrease ambient intensity |
| `P` | Increase ambient intensity |

> Press `L` again to return to **RENDER MODE**

---

### 🖼️ **RENDER MODE** (Default)

| Key | Action |
|-----|--------|
| `C` | Enter **CAMERA MODE** |
| `L` | Enter **LIGHT MODE** |
| `Q` | Save current frame as image |
| `ESC` | Exit program |
| **Click** | Select object (enter **OBJECT MODE**) |

---

## 🛠️ Build & Run

```bash
make
./miniRT scenes/sp.rt
```

### Scene Files

Example scenes are located in the `scenes/` directory:
- `sp.rt` — Simple sphere
- `solar.rt` — Solar system
- `cone.rt` — Cone showcase
- `diamond.rt` — Diamond mesh model
- And more!

---
