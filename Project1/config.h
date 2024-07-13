#pragma once
namespace Game {
	namespace Config {
		inline constexpr Vector windowSize(800, 600);
		inline constexpr Vector sceneSize(800, 600);
		inline constexpr Vector scenePosition((windowSize.y - sceneSize.y / 2),(windowSize.x - sceneSize.x / 2));
	}
}