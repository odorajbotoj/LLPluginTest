#include "ll_plugin_test/LLPluginTest.h"
#include "ll_plugin_test/tests/tests.h"

#include <memory>

#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/RegisterHelper.h"

namespace ll_plugin_test {

static std::unique_ptr<LLPluginTest> instance;

LLPluginTest& LLPluginTest::getInstance() { return *instance; }

bool LLPluginTest::load() {
    getSelf().getLogger().info("Loading...");
    // Code for loading the plugin goes here.
    return true;
}

bool LLPluginTest::enable() {
    auto& logger = getSelf().getLogger();
    logger.info("Enabling...");
    // Code for enabling the plugin goes here.
    command::regCmd(logger);
    perm_test::permTest();
    return true;
}

bool LLPluginTest::disable() {
    getSelf().getLogger().info("Disabling...");
    // Code for disabling the plugin goes here.
    return true;
}

} // namespace ll_plugin_test

LL_REGISTER_PLUGIN(ll_plugin_test::LLPluginTest, ll_plugin_test::instance);
