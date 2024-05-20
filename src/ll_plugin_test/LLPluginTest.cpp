#include "ll_plugin_test/LLPluginTest.h"
#include "ll_plugin_test/tests/tests.h"

#include <filesystem>
#include <fstream>
#include <memory>

#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/RegisterHelper.h"

namespace {
std::ofstream myFile;
}

namespace ll_plugin_test {

static std::unique_ptr<LLPluginTest> instance;

LLPluginTest& LLPluginTest::getInstance() { return *instance; }

bool LLPluginTest::load() {
    // Code for loading the plugin goes here.
    auto& logger = getSelf().getLogger();
    logger.info("Loading...");
    const auto& dataPath = getSelf().getDataDir();
    logger.info("dataPath: {}", dataPath);
    try {
        std::filesystem::create_directories(dataPath);
    } catch (const std::filesystem::filesystem_error&) {
        logger.error("cannot create directories.");
        return false;
    }
    myFile.open(getSelf().getDataDir() / u8"测试.txt");
    if (!myFile.is_open()) {
        logger.error("cannot create file.");
        return false;
    }
    return true;
}

bool LLPluginTest::enable() {
    // Code for enabling the plugin goes here.
    auto& logger = getSelf().getLogger();
    logger.info("Enabling...");
    logger.info("测试中文信息……");
    myFile << "中文测试123" << std::endl;
    command::regCmd(logger);
    perm_test::permTest();
    run_cmd::runCmdTest();
    return true;
}

bool LLPluginTest::disable() {
    getSelf().getLogger().info("Disabling...");
    // Code for disabling the plugin goes here.
    myFile.close();
    return true;
}

} // namespace ll_plugin_test

LL_REGISTER_PLUGIN(ll_plugin_test::LLPluginTest, ll_plugin_test::instance);
