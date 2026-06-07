#include "naida/log.hh"
#include "spdlog/common.h"
#include <spdlog/logger.h>
#include <memory>

namespace naida
{
static std::shared_ptr<spdlog::logger> init_logger()
{
    spdlog::set_pattern("[%H:%M:%S:%f] [Thread %t] [%l] %v");
    auto logger = spdlog::stdout_color_mt("naida");
    logger->set_level(spdlog::level::trace);
    return logger;
}

std::shared_ptr<spdlog::logger> get_logger()
{
    static auto logger = init_logger();
    return logger;
}
} // namespace naida