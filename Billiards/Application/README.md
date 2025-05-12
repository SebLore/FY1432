## Application

Main project for the billiards project.

The application is meant to render a simulation of a game of billiards/pool. I don't actually know 
what the rules to pool are, so it's a simple game of 'get all the balls in the holes'. The main 
focus of the application is to simulate the physics of a real game of pool.

### Project Layout

```
Application
├── include
├── assets
├── README.md
├── shader
├── src

```

#### External dependencies 

External dependencies used for the application, all rights reserved:

- stb: for stb-image.h etc. Loading textuers
- GLFW: Window management and input handling
- glad: OpenGL loader and function pointer management
- glm: math library
- ImGui: Debug GUI and backends
- assimp: for loading 3D models

### Physics

Since the game is meant as a simulation of billiards, the main focus lies in the physics.
For billiards, the physics calculations in order of relevancy are:

1. **Collisions:**
	a. Ball-Ball
	b. Ball-Cue
	c. Ball-Railing
2. **Roll-slide friction** 
3. Friction



#### Data points

For this game of Billiards, I used data available freely on the site [drdavepoolinfo.com](https://drdavepoolinfo.com/faq/physics/physical-properties/). 
I haven't verified if they are entirely correct but they seem reasonable.


##### Table 1: Parameters for the billiard ball

| Parameters | Diameter (cm) | Radius R(cm) | Mass m (grams) | Angular inertia $I$ ($g\cdot cm^2$) |
| ---------- | ------------- | -----------  | -------------- | ----------------------------------- |
| Ball       | 5.715         | 2,8575       | 170            | 555*                                |

\*according to the formula for solid spheres, $I=\frac{2}{5}\cdot m\cdot R^2.$

##### Table 2: The restitution coefficient $e$ between a ball and the kinds of bodies it's expected to collide with

| Restitution (e)		 | Ball        | Railing      | Table  | Cue          |
| ---------------------- | ----------- | --------- | --------- | ----------- |
| Ball                   | 0.92 - 0.98 | 0.6 - 0.9 | 0.5 - 0.7 | 0.71 - 0.75 |

##### Table 3: Coefficient of friction $\mu$ between a ball and the surfaces its expected to interact with

| Frictional coefficient ($\mu$) | Ball        | Table surface glide | Table surface roll | Cue|
| ------------------------------ | ----------- | ------------------- | ------------------ | --- |
| Ball                           | 0.03 - 0.08 | 0.15 - 0.4 (0.2\*)  | 0.005 - 0.015      | 0.6 |

\*most common according to davepoolinfo


##### Table 4: The rate of deceleration for the ball's spin while in contact with the surface of the table.

| Rate of deceleration for spin (rad/$s^2$) | Table surface|
| --------------------------------- | -------- |
| Ball                              | 5-15     |

##### Calculation of the moment of inertia

$$\begin{array}l 
I &= \frac{2}{5}\cdot m \cdot R^2 \\
I &= \frac{2}{5}\cdot170\cdot(2.8575)^2 \approx 555\ g\cdot cm^2\\
\end{array}$$

$1 g \cdot cm^2 = 10^{-7}\ kg\cdot m^2$
