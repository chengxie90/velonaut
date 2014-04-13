App.onConfigLoaded( {
scenes = {
  {
    name = "game",
    camera = {
      position = { x=0, y=0, z=40 },
      lookat = { x=0, y=0, z=40 }
    },
    objects = {
      {
        name = "actor",
        components = {
          transform = {
            position = { x=0, y=10, z=-2 },
            rotation = { x=0.1, y=0, z=0 },
            scale	 = { x=1, y=1, z=1 },
          },
          physics = {
            mode = "rigid",
            mass = { x=0, y=10, z=-2 }
          },
          collider = {
            mode = "box",
            size = { x=10, y=10, z=10 }
          },
          renderer  = {
            mesh = "meshfile.mesh",										
            shadows = true,
            material = {
              color = { r=255, g=0, b=0 },
              vert  = "vertexshader.vert",
              frag  = "fragshader.frag"
            }
          },
          behavior = {
            script = "actor_behavior.lua"										
          },
          camera = {
            position = { x=0, y=0, z=40 },
            lookat = { x=0, y=0, z=40 }
          }
        }
      }
    }
  }
}
})
