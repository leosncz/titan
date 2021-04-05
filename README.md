# Titan Engine

**Titan Engine** is a powerfull & lightweight game engine.

## Example
Shadow example :
![Example](example3.png "Example of actual titan rendering")
PBR example :
![Example](example5.png "Example of actual titan rendering")
![Example](example4.png "Example of actual titan rendering")

## Actual supported features:
- Runs on Windows, Mac & Linux (32-64 bits)
- Infinite number of lights
- Directionnal, spot and point light supported (spot light broken now due to defered shading)
- Physically Based Rendering (PBR)
- Albedo map (up to 3 textures on a single object)
- Metallic map (broken now due to defered shading, fixing it)
- Roughness map (broken now due to defered shading, fixing it)
- Normal map
- Gamma correction
- HDR
- Defered Shading
- Dynamic shadows
- OBJ Model loader
- Automatic texture cache
- GUI

## Currently working on (top is prioritized) :
- AO map (priority)
- SSAO (priority)
- TitanScripting to enable users exporting their game using TitanEngine
- Physics system
- Infinite number of shadows
- Dynamic light frustrum

## Build
In order to use & build the project with this library, you must install the following libraries :
- GLFW
- OpenGL
- GLEW
- GLM </br>
If you use the sln solution, keep in mind that everything is 64-bit oriented.

## License
Please contact me before considering using any code from this repository.