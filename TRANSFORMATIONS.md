# Understanding 2D Transformations and the Sprite Renderer

This document explains how the vertex shader, model matrix, and orthographic projection work together to render sprites in 2D space.

## Table of Contents
1. [The Big Picture](#the-big-picture)
2. [Coordinate Systems](#coordinate-systems)
3. [The Vertex Data](#the-vertex-data)
4. [The Model Matrix](#the-model-matrix)
5. [The Projection Matrix](#the-projection-matrix)
6. [The Transformation Pipeline](#the-transformation-pipeline)
7. [Step-by-Step Example](#step-by-step-example)

---

## The Big Picture

When you call `sr_draw_sprite()`, here's what happens:

```
Unit Quad (0,0 to 1,1) 
    ↓ [Model Matrix]
Transformed Quad (positioned, rotated, scaled)
    ↓ [Projection Matrix]
Screen Coordinates (pixels)
    ↓ [GPU Rasterization]
Pixels on Screen
```

The vertex shader performs this transformation:
```glsl
gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
```

**Matrix multiplication order matters!** We read from right to left:
1. First: `model` transforms the vertex
2. Then: `projection` transforms the result to screen space

---

## Coordinate Systems

### 1. **Model Space (Local Space)**
- The unit quad vertices: `(0,0)` to `(1,1)`
- This is the "raw" vertex data before any transformations
- Think of it as the sprite's "natural" size and position

### 2. **World Space**
- After applying the model matrix
- The sprite is positioned, rotated, and scaled in your game world
- Still in "game units" (not pixels yet)

### 3. **Screen Space (Clip Space)**
- After applying the projection matrix
- Coordinates are in pixels: `(0,0)` top-left to `(800,600)` bottom-right
- This is what gets rendered to the screen

---

## The Vertex Data

In `sr_create()`, we define a unit quad:

```c
float vertices[] = { 
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,  // Bottom-left
    1.0f, 0.0f, 1.0f, 0.0f,  // Top-right
    0.0f, 0.0f, 0.0f, 0.0f,  // Top-left
    
    0.0f, 1.0f, 0.0f, 1.0f,  // Bottom-left
    1.0f, 1.0f, 1.0f, 1.0f,  // Bottom-right
    1.0f, 0.0f, 1.0f, 0.0f   // Top-right
};
```

**Why a unit quad?**
- It's normalized: always `(0,0)` to `(1,1)`
- We scale it to any size we want using the model matrix
- Texture coordinates match perfectly: `(0,0)` to `(1,1)`

Visual representation:
```
(0,0) ─────────── (1,0)
  │                  │
  │      Unit        │
  │      Quad        │
  │                  │
(0,1) ─────────── (1,1)
```

---

## The Model Matrix

The model matrix transforms the unit quad from model space to world space. It's built in `sr_draw_sprite()`:

```c
mat4 model = GLM_MAT4_IDENTITY_INIT;  // Start with identity matrix

// Step 1: Move to desired position
glm_translate(model, (vec3){pos[0], pos[1], 0.0f});

// Step 2: Move to rotation center (half the size)
glm_translate(model, (vec3){0.5f * size[0], 0.5f * size[1], 0.0f});

// Step 3: Rotate around the center
glm_rotate(model, glm_rad(rotate), (vec3){0.0f, 0.0f, 1.0f});

// Step 4: Move back from rotation center
glm_translate(model, (vec3){-0.5f * size[0], -0.5f * size[1], 0.0f});

// Step 5: Scale to desired size
glm_scale(model, (vec3){size[0], size[1], 1.0f});
```

### What Each Function Call Actually Does (CPU Side)

**On the CPU:** Each function call multiplies the current `model` matrix by a transformation matrix. You're building up one final matrix.

```c
mat4 model = GLM_MAT4_IDENTITY_INIT;  // model = Identity matrix

glm_translate(model, T1);  // model = model * T1  → model = T1
glm_translate(model, T2);  // model = model * T2  → model = T1 * T2
glm_rotate(model, R);      // model = model * R   → model = T1 * T2 * R
glm_scale(model, S);       // model = model * S   → model = T1 * T2 * R * S
```

**After all calls, `model` is a single matrix:** `model = T1 * T2 * R * S`

This single matrix is sent to the GPU.

### What Happens on the GPU

**On the GPU:** The vertex shader receives the single `model` matrix and does:
```glsl
gl_Position = projection * model * vertex;
```

The GPU multiplies `projection * model * vertex` as a single operation. It doesn't decompose `model` - it's just one matrix multiplication.

**But here's the key:** Even though `model` is one matrix, it was built as `T1 * T2 * R * S`. When you multiply a matrix chain like `T1 * T2 * R * S * vertex`, mathematically it's equivalent to applying the transformations in reverse order:

- The rightmost matrix (`S`) affects the vertex first
- Then `R` affects the result
- Then `T2` affects that result  
- Then `T1` affects that result

**Why?** Because matrix multiplication is right-associative. `T1 * T2 * R * S * vertex` means:
1. First compute: `S * vertex` (scale happens first)
2. Then: `R * (S * vertex)` (rotate the scaled vertex)
3. Then: `T2 * (R * (S * vertex))` (translate the rotated, scaled vertex)
4. Then: `T1 * (T2 * (R * (S * vertex)))` (final translate)

**So the order you build the matrix on CPU determines the order transformations are applied on GPU:**
- Last thing you multiply in (scale) → First thing applied to vertex
- First thing you multiply in (translate) → Last thing applied to vertex

### Why This Order?

Think about what happens to a vertex as it goes through each transformation. The transformations are applied **in reverse order** from how we write them in code.

When we write:
```c
glm_scale(model, S);        // Written first
glm_rotate(model, R);       // Written second  
glm_translate(model, T2);   // Written third
glm_translate(model, T1);   // Written last
```

The vertex actually gets transformed in this order:
1. **Scale first** (S) - makes the unit quad bigger
2. **Rotate second** (R) - rotates the scaled quad
3. **Translate third** (T2) - moves the rotated quad
4. **Translate last** (T1) - moves it to final position

**Why reverse?** Because matrix multiplication works like function composition. When we do `T1 * T2 * R * S * vertex`, the rightmost matrix (S) is applied to the vertex first, then R, then T2, then T1.

Think of it like putting on clothes: you put on your shirt (scale), then your jacket (rotate), then walk to the door (translate). But if you wrote it as code, you'd list the final action last, even though it happens last!

### Concrete Example with Our Actual Code

**On the CPU - Building the Model Matrix:**

```c
mat4 model = GLM_MAT4_IDENTITY_INIT;  // model = Identity

// We call these functions in this order:
glm_translate(model, (vec3){250.0f, 100.0f, 0.0f});      // T1: translate to position
glm_translate(model, (vec3){150.0f, 200.0f, 0.0f});      // T2: translate to center
glm_rotate(model, glm_rad(45.0f), (vec3){0.0f, 0.0f, 1.0f});  // R: rotate
glm_translate(model, (vec3){-150.0f, -200.0f, 0.0f});   // T3: translate back
glm_scale(model, (vec3){300.0f, 400.0f, 1.0f});         // S: scale
```

**What happens on CPU (building the matrix):**
```c
// After each call, model becomes:
// Step 1: model = T1
// Step 2: model = T1 * T2
// Step 3: model = T1 * T2 * R
// Step 4: model = T1 * T2 * R * T3
// Step 5: model = T1 * T2 * R * T3 * S  ← Final single matrix
```

**Final result:** `model` is now a single 4x4 matrix: `T1 * T2 * R * T3 * S`

This single matrix is sent to the GPU.

**On the GPU - Applying the Matrix:**

The vertex shader does: `gl_Position = projection * model * vertex`

Since `model = T1 * T2 * R * T3 * S`, this becomes:
```
gl_Position = projection * (T1 * T2 * R * T3 * S) * vertex
```

**When the GPU multiplies this chain, it evaluates right-to-left:**
1. `S * vertex` first (scale happens first)
2. Then `T3 * (S * vertex)` (translate back)
3. Then `R * (T3 * (S * vertex))` (rotate)
4. Then `T2 * (R * (T3 * (S * vertex)))` (translate to center)
5. Then `T1 * (T2 * (R * (T3 * (S * vertex))))` (translate to position)
6. Finally `projection * (T1 * ...)` (convert to screen space)

**Visual example with vertex `(0.5, 0.5)`:**
- Start: `(0.5, 0.5)` (unit quad center)
- After S: `(150, 200)` ✓ Scaled to 300×400
- After T3: `(0, 0)` ✓ Moved center to origin
- After R: `(0, 0)` ✓ Rotated around origin
- After T2: `(150, 200)` ✓ Moved to rotation center
- After T1: `(400, 300)` ✓ Final position
- After projection: Screen coordinates

**Key insight:** We call translate first in the code, but because matrix multiplication is right-associative, the last matrix we multiply in (scale) is the first one applied to the vertex!

### Step-by-Step Breakdown

Let's say we want to draw a sprite at position `(250, 100)` with size `(300, 400)` and rotation `45°`:

#### 1. **Identity Matrix** (starting point)
```
[1 0 0 0]
[0 1 0 0]
[0 0 1 0]
[0 0 0 1]
```
This does nothing - it's the "do nothing" matrix.

#### 2. **Translate to Position** `(250, 100)`
```
[1 0 0 250]
[0 1 0 100]
[0 0 1 0  ]
[0 0 0 1  ]
```
Moves the quad so its top-left corner is at `(250, 100)`.

#### 3. **Translate to Rotation Center** `(150, 200)` (half of size)
```
[1 0 0 400]  // 250 + 150
[0 1 0 300]  // 100 + 200
[0 0 1 0  ]
[0 0 0 1  ]
```
Moves the quad so its center is at the rotation point.

#### 4. **Rotate** `45°` around Z-axis
```
[cos(45°) -sin(45°) 0 400]
[sin(45°)  cos(45°) 0 300]
[0         0        1 0  ]
[0         0        0 1  ]
```
Rotates the quad around its center.

#### 5. **Translate Back** `(-150, -200)`
```
[cos(45°) -sin(45°) 0 400-150*cos(45°)+200*sin(45°)]
[sin(45°)  cos(45°) 0 300-150*sin(45°)-200*cos(45°)]
[0         0        1 0                              ]
[0         0        0 1                              ]
```
Moves the rotation center back to where we want the sprite center.

#### 6. **Scale** `(300, 400)`
```
[300*cos(45°) -300*sin(45°) 0 ...]
[400*sin(45°)  400*cos(45°) 0 ...]
[0             0            1 0  ]
[0             0            0 1  ]
```
Scales the unit quad to the desired size.

**Result:** A 300×400 sprite, rotated 45°, with its center at `(250+150, 100+200) = (400, 300)`.

### Why Translate to Center, Rotate, Then Translate Back?

**Problem:** If we rotate around the top-left corner `(0,0)` without translating to center first, the sprite orbits around that corner and moves to a different position on screen.

```
Rotating around CORNER (bad - sprite moves!):

    Before:                    After 45° rotation:
    +-------+                  X (pivot point)
    |       |                     \
    |   *   |  <- center           \    +-------+
    |       |                       \   |   *   |  <- center moved!
    +-------+                        \  |       |
    X <- pivot (corner)                \ +-------+
                                       (sprite moved up and right)
```

The sprite's center (marked `*`) moves because we're rotating around the corner (marked `X`), not the center. The pivot stays in place, but the sprite orbits around it.

**Solution:** By translating to the center first, rotating, then translating back, we rotate around the sprite's center, so it stays in the same position.

```
Rotating around CENTER (good - sprite stays in place!):

    Before:                    After 45° rotation:
    +-------+                  +-------+
    |       |                  |       |
    |   X   |  <- pivot        |   X   |  <- pivot (same spot)
    |       |                  |       |
    +-------+                  +-------+
    (sprite here)              (sprite still here, just rotated)
```

The sprite rotates in place because the pivot point `X` is at its center. The center position doesn't change - only the orientation (rotation angle) changes. The sprite stays exactly where it is on screen.

---

## The Projection Matrix

The projection matrix converts world coordinates to screen (pixel) coordinates. It's created once in `g_init()`:

```c
mat4 projection = {0};
glm_ortho(0.0f, SCR_WIDTH, SCR_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
```

### `glm_ortho()` Parameters

```c
glm_ortho(left, right, bottom, top, near, far, projection)
```

- **left = 0.0f**: Left edge of screen (X = 0)
- **right = 800.0f**: Right edge of screen (X = 800)
- **bottom = 600.0f**: Bottom edge of screen (Y = 600)
- **top = 0.0f**: Top edge of screen (Y = 0)
- **near = -1.0f**: Near clipping plane (for 2D, not really used)
- **far = 1.0f**: Far clipping plane (for 2D, not really used)

**Note:** `bottom > top` means Y increases downward (screen coordinates).

### What the Orthographic Matrix Does

The orthographic projection matrix looks like this:

```
[2/(right-left)  0               0  -(right+left)/(right-left)]
[0               2/(top-bottom)   0  -(top+bottom)/(top-bottom)]
[0               0               -2/(far-near)  -(far+near)/(far-near)]
[0               0                0   1                          ]
```

For our values `(0, 800, 600, 0, -1, 1)`:

```
[2/800   0       0  -1]
[0      -2/600   0   1]
[0       0      -1   0]
[0       0       0   1]
```

### How It Works

1. **X-axis**: Maps `[0, 800]` to `[-1, 1]` (OpenGL clip space)
   - X = 0 → -1 (left edge)
   - X = 400 → 0 (center)
   - X = 800 → 1 (right edge)

2. **Y-axis**: Maps `[0, 600]` to `[1, -1]` (inverted because Y increases down)
   - Y = 0 → 1 (top edge)
   - Y = 300 → 0 (center)
   - Y = 600 → -1 (bottom edge)

3. **Z-axis**: For 2D, we keep Z = 0, which maps to 0 in clip space

The GPU then converts clip space `[-1, 1]` to screen pixels `[0, width/height]`.

---

## The Transformation Pipeline

Here's the complete flow from vertex to screen:

### 1. **Vertex Shader Input**
```glsl
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
```

For a unit quad vertex at `(0.5, 0.5)`:
- `vertex.xy = (0.5, 0.5)` - position in model space
- `vertex.zw = (0.5, 0.5)` - texture coordinates (passed through unchanged)

### 2. **Model Transformation**
```glsl
vec4 worldPos = model * vec4(vertex.xy, 0.0, 1.0);
```

Transforms from model space to world space:
- `(0.5, 0.5)` → `(400, 300)` (after all model matrix operations)

### 3. **Projection Transformation**
```glsl
gl_Position = projection * worldPos;
```

Transforms from world space to clip space:
- `(400, 300)` → approximately `(0, 0)` in clip space (center of screen)

### 4. **GPU Rasterization**
The GPU converts clip space to screen pixels:
- Clip space `(0, 0)` → Screen pixel `(400, 300)`

---

## Step-by-Step Example

Let's trace a complete example: drawing a sprite at `(250, 100)` with size `(300, 400)` and no rotation.

### Initial Vertex
Unit quad vertex: `(0.5, 0.5)` (center of the unit quad)

### After Model Matrix

The model matrix operations (in reverse order of application):
1. **Scale** `(300, 400)`: `(0.5, 0.5)` → `(150, 200)`
2. **Translate back** `(-150, -200)`: `(150, 200)` → `(0, 0)`
3. **Rotate** `0°`: `(0, 0)` → `(0, 0)` (no change)
4. **Translate to center** `(150, 200)`: `(0, 0)` → `(150, 200)`
5. **Translate to position** `(250, 100)`: `(150, 200)` → `(400, 300)`

**Result in world space:** `(400, 300)`

### After Projection Matrix

The projection matrix converts `(400, 300)` to clip space:

```
X: 400 → (400 - 400) / 400 = 0.0
Y: 300 → (300 - 300) / 300 = 0.0
```

**Result in clip space:** `(0.0, 0.0)` - the center of the screen!

### Final Screen Position

The GPU converts clip space `(0.0, 0.0)` to screen pixel `(400, 300)`.

**The sprite's center is at screen pixel (400, 300), which is the center of an 800×600 screen!**

---

## Key Takeaways

1. **Unit Quad**: We always start with a `(0,0)` to `(1,1)` quad, then scale it.

2. **Model Matrix Order**: Transformations are applied right-to-left:
   - Scale first (makes the quad bigger)
   - Rotate second (around the center)
   - Translate last (moves it to position)

3. **Rotation Center**: We translate to the center, rotate, then translate back to rotate around the sprite's center, not its corner.

4. **Projection Matrix**: Converts world coordinates (game units) to screen coordinates (pixels). It's set up once and reused for all sprites.

5. **Matrix Multiplication**: `projection * model * vertex` means:
   - First apply `model` (position, rotate, scale)
   - Then apply `projection` (convert to screen space)

---

## Visual Summary

```
Model Space          World Space           Screen Space
(0,0) to (1,1)  →   Positioned & Scaled  →  Pixels (0,0) to (800,600)
     ↓                      ↓                        ↓
  Unit Quad         300×400 Sprite at      Rendered at
                     (250,100)             screen position
```

The vertex shader does all this math for every vertex, 60+ times per second!

