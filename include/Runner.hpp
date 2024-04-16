#pragma once

#include "Defines.hpp"

namespace wfe {
	/// @brief Runs the requested function on its constructor.
	class Runner {
	public:
		/// @brief Runs the given function.
		/// @tparam F The function's type.
		/// @tparam ...Args The function's argument types.
		/// @param func The function to run.
		/// @param ...args The args to run the function with.
		template<class F, class... Args>
		Runner(F&& func, Args... args) {
			func(args...);
		}
	};

/// @brief Runs the given function with the given params, using a runner variable with the given prefix.
/// @param name The prefix of the runner variable's name, saves as runner_name.
/// @param func The function to run.
#define WFE_RUN_FULL(name, func, ...) static wfe::Runner WFE_MACRO_CONCAT(runner_, name) { func, __VA_ARGS__ };
/// @brief Runs the given function with the given params.
/// @param func The function to run.
#define WFE_RUN(func, ...) WFE_RUN_FULL(__LINE__, func, __VA_ARGS__)
}