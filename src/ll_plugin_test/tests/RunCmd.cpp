#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/service/Bedrock.h"
#include "mc/deps/core/string/HashedString.h"
#include "mc/locale/I18n.h"
#include "mc/locale/Localization.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandOutputType.h"
#include "mc/server/commands/CommandPermissionLevel.h"
#include "mc/server/commands/MinecraftCommands.h"
#include "mc/server/commands/ServerCommandOrigin.h"
#include "mc/world/Minecraft.h"
#include "mc/world/level/Command.h"
#include "mc/world/level/Level.h"


#include <string>

namespace ll_plugin_test::run_cmd {
struct RunCmdRes {
    bool        success;
    std::string output;
};
RunCmdRes RunCmd(HashedString& cmd) {
    auto origin =
        ServerCommandOrigin("Server", ll::service::getLevel()->asServer(), CommandPermissionLevel::Internal, 0);
    auto command = ll::service::getMinecraft()->getCommands().compileCommand(
        cmd,
        origin,
        (CurrentCmdVersion)CommandVersion::CurrentVersion,
        [](std::string const& err) { (void)err; }
    );
    CommandOutput                        output(CommandOutputType::AllOutput);
    std::string                          outputStr;
    RunCmdRes                            res;
    static std::shared_ptr<Localization> localization =
        getI18n().getLocaleFor(getI18n().getCurrentLanguage()->getFullLanguageCode());
    if (command) {
        command->run(origin, output);
        for (auto msg : output.getMessages()) {
            outputStr += getI18n().get(msg.getMessageId(), msg.getParams(), localization).append("\n");
        }
        if (output.getMessages().size()) {
            outputStr.pop_back();
        }
        res.success = output.getSuccessCount() ? true : false;
        res.output  = outputStr;
        return res;
    } else {
        res.success = false;
        res.output  = "";
        return res;
    }
}

void runCmdTest() {
    auto& cmd = ll::command::CommandRegistrar::getInstance()
                    .getOrCreateCommand("trc", "test run command", CommandPermissionLevel::Any);
    cmd.overload().execute([](CommandOrigin const& origin, CommandOutput& output) {
        (void)origin;
        output.success("begin");
        HashedString command1("say hello你好");
        RunCmdRes    res1 = RunCmd(command1);
        output.success("success: {}\noutput: {}", res1.success, res1.output);
        HashedString command2("testfor @e");
        RunCmdRes    res2 = RunCmd(command2);
        output.success("success: {}\noutput: {}", res2.success, res2.output);
        output.success("end");
    });
}
} // namespace ll_plugin_test::run_cmd