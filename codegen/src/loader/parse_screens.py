from common.util import print_json, ensure_list

def parse_screens(config: dict):

    # process screen data
    for screen_data in config["screens"].values():

        renderer = screen_data.get("renderer", None)

        if renderer is not None:
            renderer = renderer.strip().lower()

            if renderer == "lvgl-simple":
                pass # nothing to do
                
            elif renderer == "lvgl-gui":
                # get gui filename
                renderer_gui = screen_data.get("renderer_gui", None)
                if renderer_gui is None:
                    raise ValueError("renderer 'lvgl-gui' requires additional attribute 'renderer_gui'")
                screen_data["renderer_gui"] = renderer_gui

                # get simple gui name
                gui_name = renderer_gui.replace(".xml", "").strip()
                screen_data["gui"] = config["guis"][gui_name]
        
        screen_data["renderer"] = renderer
        
        if "render_fns" in screen_data:
            screen_data["render_fns"] = ensure_list(screen_data["render_fns"])
        else:
            screen_data["render_fns"] = None