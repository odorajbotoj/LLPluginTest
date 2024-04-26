#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"

namespace ll_plugin_test::perm_test {
void permTest() {
    // 注册命令A
    auto& cmdA =
        ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
            "tcpa",
            "test command perm Any",
            CommandPermissionLevel::Any
        );
    cmdA.overload().execute<[](CommandOrigin const& origin, CommandOutput& output){
        (void)origin;
        output.success("Perm Any");
    }>();

    // 注册命令B
    auto& cmdB =
        ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
            "tcpb",
            "test command perm GameDirectors",
            CommandPermissionLevel::GameDirectors
        );
    cmdB.overload().execute<[](CommandOrigin const& origin, CommandOutput& output){
        (void)origin;
        output.success("Perm GameDirectors");
    }>();

    // 注册命令C
    auto& cmdC =
        ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
            "tcpc",
            "test command perm Admin",
            CommandPermissionLevel::Admin
        );
    cmdC.overload().execute<[](CommandOrigin const& origin, CommandOutput& output){
        (void)origin;
        output.success("Perm Admin");
    }>();

    // 注册命令D
    auto& cmdD =
        ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
            "tcpd",
            "test command perm Host",
            CommandPermissionLevel::Host
        );
    cmdD.overload().execute<[](CommandOrigin const& origin, CommandOutput& output){
        (void)origin;
        output.success("Perm Host");
    }>();

    // 注册命令E
    auto& cmdE =
        ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
            "tcpe",
            "test command perm Owner",
            CommandPermissionLevel::Owner
        );
    cmdE.overload().execute<[](CommandOrigin const& origin, CommandOutput& output){
        (void)origin;
        output.success("Perm Owner");
    }>();

    // 注册命令F
    auto& cmdF =
        ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
            "tcpf",
            "test command perm Internal",
            CommandPermissionLevel::Internal
        );
    cmdF.overload().execute<[](CommandOrigin const& origin, CommandOutput& output){
        (void)origin;
        output.success("Perm Internal");
    }>();
}
} // namespace ll_plugin_test::perm_test