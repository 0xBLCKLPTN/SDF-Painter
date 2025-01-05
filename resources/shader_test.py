import moderngl_window as mglw
import moderngl

class App(mglw.WindowConfig):
    window_size: tuple[int, int] = 800, 600
    resource_dir: str = './'

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.quad = mglw.geometry.quad_fs() # Fullscreen quad.
        self.program = self.load_program(vertex_shader='vertex_shader.glsl', fragment_shader='fragment_shader.glsl')

        self.program['u_resolution'] = self.window_size

    def on_render(self, time, frame_time):
        self.ctx.clear()
        self.quad.render(self.program)

if __name__ == '__main__':
    mglw.run_window_config(App)
