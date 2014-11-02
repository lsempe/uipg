#pragma once

#include "engine.core/game.h"
#include "engine.core/timer.h"
#include "engine.math/matrix.h"
#include "engine.render/device_direct3d.h"
#include "engine.render/viewport.h"

#include "engine.render/quad.h"

#include "SpriteBatch.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"

#include "health.h"

#include "world.h"
#include "engine.ui/container.h"
#include "engine.ui/input_cursor.h"
#include "engine.render/ovr.h"
#include "engine.render/camera.h"
#include "engine.render/camera_ovr.h"
#include "engine.render/camera_rt.h"
#include "engine.render/cube.h"

#include "engine.ui/analog_gauge.h"

#include "engine.render/text.h"

#include "engine.core/runtime_variable.h"

namespace input
{
	class input_state;
	class mouse;
}

namespace ui
{
	class element_damage_indicator;
	class element_world_indicator;
	class element_compass;
	class element_minimap;
}

class example_game : public game::core
{
public:
	example_game(HWND& windowHandle, int clientWidth, int clientHeight);
	virtual ~example_game();

	virtual void Initialize();
	virtual void Update();

	virtual void SetViewport(const render::viewport& viewport);
	virtual void SetProjection(const math::matrix& projection);

protected:

	void Update(double deltaTime);
	
	virtual void PreRender();
	virtual void Render();
	virtual void Present();

	render::device_direct3d m_device;

	std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	struct render_settings
	{
		bool m_stereoscopic;

		render_settings();
	};

	render_settings m_renderSettings;

	std::unique_ptr<render::text> m_text;

	math::matrix m_projection;

	game::World m_world;
	
	std::unique_ptr<game::Player> m_player;
	

	std::unique_ptr<ui::container> m_uiContainer;

	std::shared_ptr<ui::core_ui> m_coreUI;

	std::shared_ptr<input::input_state> m_inputState;
	std::shared_ptr<input::mouse> m_mouse;
	std::shared_ptr<input::keyboard> m_keyboard;

	std::unique_ptr<ui::input_cursor> m_cursor;

	std::unique_ptr<render::ovr> m_ovr;
	
	std::unique_ptr<render::quad> m_testQuad;

	std::unique_ptr<ui::element_health> m_health;
	std::unique_ptr<ui::element_damage_indicator> m_damageIndicator;
	std::unique_ptr<ui::element_world_indicator> m_worldIndicators;
	std::unique_ptr<ui::element_compass> m_compass;
	std::unique_ptr<ui::element_minimap> m_minimap;	

	std::unique_ptr<render::cube> m_testCube;

	std::unique_ptr<ui::analog> m_analog;

	std::unique_ptr<render::camera_rt> m_cameraRT;
	void Draw();

	render::camera m_camera;
	render::camera_ovr m_cameraOVR;
	math::matrix m_transform;

	timer m_timer;

	std::unique_ptr<ui::console> m_console;

	runtime_variable_function m_memberFunction;
	bool OnTest(std::vector<std::wstring> parameters);

	double m_currentTime;
};

extern render::device_direct3d* GetDevice();