#include "ll/api/Logger.h"
#include "ll/api/command/CommandHandle.h"
#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/service/Bedrock.h"

#include "mc/entity/utilities/ActorType.h"
#include "mc/server/commands/CommandOrigin.h"
#include "mc/server/commands/CommandOutput.h"
#include "mc/server/commands/CommandPermissionLevel.h"
#include "mc/server/commands/CommandVersion.h"
#include "mc/server/commands/WildcardCommandSelector.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/Player.h"

#include "mc/server/commands/BlockStateCommandParam.h"
// #include "mc/server/commands/CommandCompareOperator.h"
#include "mc/server/commands/CommandFilePath.h"
#include "mc/server/commands/CommandIntegerRange.h"
#include "mc/server/commands/CommandMessage.h"
// #include "mc/server/commands/CommandOperator.h"
// #include "mc/server/commands/CommandPosition.h"
#include "mc/_HeaderOutputPredefine.h"
#include "mc/deps/core/common/bedrock/typeid_t.h"
#include "mc/deps/json/Value.h"
#include "mc/server/commands/CommandBlockName.h"
#include "mc/server/commands/CommandItem.h"
#include "mc/server/commands/CommandPositionFloat.h"
#include "mc/server/commands/CommandRawText.h"
#include "mc/server/commands/CommandSelector.h"
#include "mc/server/commands/CommandWildcardInt.h"
#include "mc/server/commands/RelativeFloat.h"
#include "mc/world/actor/ActorDefinitionIdentifier.h"
#include "mc/world/effect/MobEffect.h"


#include <memory>
#include <string>
#include <vector>


namespace ll_plugin_test::command {

void regCmd(ll::Logger& logger) {
    // 注册命令A
    auto& cmd = ll::command::CommandRegistrar::getInstance()
                    .getOrCreateCommand("testcommand", "test command", CommandPermissionLevel::Any);
    // 设置别名
    ll::service::getCommandRegistry()->registerAlias("testcommand", "testcmd");
    cmd.alias("tcmd");
    // 获取别名列表
    const auto aliasList = cmd.alias();
    for (unsigned long long i = 0; i < aliasList.size(); i++)
        logger.info("Test command alias list [{}]: {}", i, aliasList[i]);
    // 无参数指令重载
    cmd.overload().text("o").execute([](CommandOrigin const& origin, CommandOutput& output) {
        auto* entity = origin.getEntity();
        if (entity == nullptr || !entity->isType(ActorType::Player)) {
            output.error("Only players can run this command");
            return;
        }
        auto* player = static_cast<Player*>(entity);
        player->sendMessage("test command");
    });

    // 设置参数
    struct CmdParamA {
        std::vector<BlockStateCommandParam> blockStateArray;
        // CommandCompareOperator compareOperator;
        CommandFilePath     pathCommand;
        CommandIntegerRange fullIntegerRange;
        CommandMessage      messageRoot;
    };
    cmd.overload<CmdParamA>()
        .text("a")
        .required("blockStateArray")
        /* .required("compareOperator") */
        .required("pathCommand")
        .required("fullIntegerRange")
        .required("messageRoot")
        .execute([](CommandOrigin const&, CommandOutput& output, CmdParamA const& param) {
            for (unsigned long long i = 0; i < param.blockStateArray.size(); i++)
                output.success(
                    "blockStateArray[{}] = mBlockState: {}, mValue: {}, mType: {}",
                    i,
                    param.blockStateArray[i].mBlockState,
                    param.blockStateArray[i].mValue,
                    param.blockStateArray[i].mValue
                );
            // output.success("compareOperator = {}", param.compareOperator);
            output.success("pathCommand = {}", param.pathCommand.mText);
            output.success(
                "fullIntegerRange = mMinValue: {}, mMaxValue: {}, mInvert: {}",
                param.fullIntegerRange.mMinValue,
                param.fullIntegerRange.mMaxValue,
                param.fullIntegerRange.mInvert
            );
            for (unsigned long long i = 0; i < param.messageRoot.components.size(); i++)
                output.success("messageRoot[{}] = {}", i, param.messageRoot.components[i].string);
        });

    // 设置参数
    struct CmdParamB {
        // CommandOperator commandOperator;
        // CommandPosition position;
        CommandPositionFloat positionFloat;
        CommandRawText       rawtext;
    };
    cmd.overload<CmdParamB>()
        .text("b")
        /* .required("commandOperator") */
        /* .required("position") */
        .required("positionFloat")
        .required("rawtext")
        .execute([](CommandOrigin const& origin, CommandOutput& output, CmdParamB const& param) {
            // output.success("operator = {}", param.commandOperator);
            // output.success("position = {}, {}, {}", param.position.getPosition(CommandVersion::CurrentVersion,
            // origin).x, param.position.getPosition(CommandVersion::CurrentVersion, origin).y,
            // param.position.getPosition(CommandVersion::CurrentVersion, origin).z);
            output.success(
                "positionFloat = {}, {}, {}",
                origin.getExecutePosition(CommandVersion::CurrentVersion, param.positionFloat).x,
                origin.getExecutePosition(CommandVersion::CurrentVersion, param.positionFloat).y,
                origin.getExecutePosition(CommandVersion::CurrentVersion, param.positionFloat).z
            );
            /*
                pos和posfloat完全没有任何的区别
                忽悠人的
                getExecutePosition就是帮你调CommandPosition::getPosition
                可以看到executecontex重写了这个函数
                这就意味着
                你自己调用getPosition在execute的时候得到的坐标就是错误的
                所以最好用CommandOrigin::getExecutePosition
                可以看到commandversion也是有用的，不同的版本得到的结果也可能不同
                (那么什么与lse的BlockPos对应呢)第一个
                -- OEOTYAN
            */
            output.success("rawtext = {}", param.rawtext.getText());
        });

    // 设置参数
    struct CmdParamC {
        CommandSelector<Player>        selectionPlayer;
        CommandSelector<Actor>         selectionActor;
        CommandWildcardInt             wildcardInt;
        Json::Value                    jsonObject;
        RelativeFloat                  rVal;
        std::basic_string<char>        id;
        WildcardCommandSelector<Actor> wildcardSelection;
        CommandItem                    item;
        CommandBlockName               block;
    };
    cmd.overload<CmdParamC>()
        .text("c")
        .required("selectionPlayer")
        .required("selectionActor")
        .required("wildcardInt")
        .required("jsonObject")
        .required("rVal")
        .required("id")
        .required("wildcardSelection")
        .required("item")
        .required("block")
        .execute([](CommandOrigin const& origin, CommandOutput& output, CmdParamC const& param) {
            auto players = param.selectionPlayer.results(origin);
            for (auto player : players) output.success("selectionPlayer = {}", player->getRealName());
            auto actors = param.selectionActor.results(origin);
            for (auto actor : actors) output.success("selectionActor = {}", actor->getTypeName());
            output.success("wildcardInt = {}", param.wildcardInt.getValue());
            // 不太会操作jsonObject，跳过
            output.success("rVal = {}", param.rVal.getValue());
            output.success("id = {}", param.id);
            auto wildcardActors = param.wildcardSelection.results(origin);
            for (auto actor : wildcardActors) output.success("wildcardSelection = {}", actor->getTypeName());
            output.success("item = {}", param.item.getId());
            output.success("block = {}", param.block.getDescriptionId());
        });

    // 设置参数
    struct CmdParamD {
        std::string                      str;
        int                              i;
        float                            f;
        bool                             b;
        const ActorDefinitionIdentifier* adi;
        const MobEffect*                 effect;
    };
    cmd.overload<CmdParamD>()
        .text("d")
        .required("str")
        .required("i")
        .postfix("t")
        .required("f")
        .required("b")
        .required("adi")
        .required("effect")
        .execute([](CommandOrigin const&, CommandOutput& output, CmdParamD const& param) {
            output.success("str={}, i={}, f={}, b={}", param.str, param.i, param.f, param.b);
            output.success("adi = {}", param.adi->getFullName());
            output.success("effect = {}", param.effect->getDescriptionId());
        });

    // 设置参数
    enum TestEnumA : int { a, b, c };
    struct CmdParamE {
        TestEnumA ea;
        enum TestEnumB : int { d, e, f } eb;
        ::DimensionType            dim;
        std::unique_ptr<::Command> cmd;
    };
    cmd.overload<CmdParamE>().text("e").required("ea").required("eb").required("dim").optional("cmd").execute(
        [](CommandOrigin const&, CommandOutput& output, CmdParamE const& param) {
            output.success("ea = {}", param.ea);
            output.success("eb = {}", param.eb);
            output.success("dim = {}", param.dim.id);
            if (param.cmd) output.success("cmd = {}", param.cmd->getCommandName());
        }
    );
}

} // namespace ll_plugin_test::command