// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Message.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppSelectOption, _SC("SqDppSelectOption"))
SQMOD_DECL_TYPENAME(SqDppComponent, _SC("SqDppComponent"))
SQMOD_DECL_TYPENAME(SqDppEmbedFooter, _SC("SqDppEmbedFooter"))
SQMOD_DECL_TYPENAME(SqDppEmbedImage, _SC("SqDppEmbedImage"))
SQMOD_DECL_TYPENAME(SqDppEmbedProvider, _SC("SqDppEmbedProvider"))
SQMOD_DECL_TYPENAME(SqDppEmbedAuthor, _SC("SqDppEmbedAuthor"))
SQMOD_DECL_TYPENAME(SqDppEmbedField, _SC("SqDppEmbedField"))
SQMOD_DECL_TYPENAME(SqDppEmbed, _SC("SqDppEmbed"))
SQMOD_DECL_TYPENAME(SqDppReaction, _SC("SqDppReaction"))
SQMOD_DECL_TYPENAME(SqDppAttachment, _SC("SqDppAttachment"))
SQMOD_DECL_TYPENAME(SqDppSticker, _SC("SqDppSticker"))
SQMOD_DECL_TYPENAME(SqDppStickerPack, _SC("SqDppStickerPack"))
SQMOD_DECL_TYPENAME(SqDppMessage, _SC("SqDppMessage"))

// ------------------------------------------------------------------------------------------------
void Register_DPP_Message(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("SelectOption"),
        Class< DpSelectOption, NoCopy< DpSelectOption > >(vm, SqDppSelectOption::Str)
        .Ctor()
        .Ctor< StackStrF &, StackStrF &, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppSelectOption::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpSelectOption::IsValid)
        .Prop(_SC("Label"), &DpSelectOption::GetLabel, &DpSelectOption::SetLabel)
        .Prop(_SC("Value"), &DpSelectOption::GetValue, &DpSelectOption::SetValue)
        .Prop(_SC("Description"), &DpSelectOption::GetDescription, &DpSelectOption::SetDescription)
        .Prop(_SC("IsDefault"), &DpSelectOption::IsDefault, &DpSelectOption::SetDefault)
        .Prop(_SC("IsAnimated"), &DpSelectOption::IsAnimated, &DpSelectOption::SetAnimated)
        .Prop(_SC("EmojiName"), &DpSelectOption::GetEmojiName, &DpSelectOption::SetEmojiName)
        .Prop(_SC("EmojiID"), &DpSelectOption::GetEmojiID, &DpSelectOption::SetEmojiID)
        // Member Methods
        .FmtFunc(_SC("SetLabel"), &DpSelectOption::ApplyLabel)
        .FmtFunc(_SC("SetValue"), &DpSelectOption::ApplyValue)
        .FmtFunc(_SC("SetDescription"), &DpSelectOption::ApplyDescription)
        .FmtFunc(_SC("SetEmoji"), &DpSelectOption::SetEmoji)
        .FmtFunc(_SC("SetDefault"), &DpSelectOption::SetDefault)
        .FmtFunc(_SC("SetAnimated"), &DpSelectOption::SetAnimated)
        .FmtFunc(_SC("SetEmojiName"), &DpSelectOption::ApplyEmojiName)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Component"),
        Class< DpComponent, NoConstructor< DpComponent > >(vm, SqDppComponent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppComponent::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpComponent::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedFooter"),
        Class< DpEmbedFooter, NoConstructor< DpEmbedFooter > >(vm, SqDppEmbedFooter::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedFooter::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedFooter::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedImage"),
        Class< DpEmbedImage, NoConstructor< DpEmbedImage > >(vm, SqDppEmbedImage::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedImage::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedImage::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedProvider"),
        Class< DpEmbedProvider, NoConstructor< DpEmbedProvider > >(vm, SqDppEmbedProvider::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedProvider::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedProvider::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedAuthor"),
        Class< DpEmbedAuthor, NoConstructor< DpEmbedAuthor > >(vm, SqDppEmbedAuthor::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedAuthor::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedAuthor::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedField"),
        Class< DpEmbedField, NoConstructor< DpEmbedField > >(vm, SqDppEmbedField::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedField::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedField::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Embed"),
        Class< DpEmbed, NoConstructor< DpEmbed > >(vm, SqDppEmbed::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbed::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbed::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Reaction"),
        Class< DpReaction, NoConstructor< DpReaction > >(vm, SqDppReaction::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppReaction::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpReaction::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Attachment"),
        Class< DpAttachment, NoConstructor< DpAttachment > >(vm, SqDppAttachment::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppAttachment::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpAttachment::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Sticker"),
        Class< DpSticker, NoConstructor< DpSticker > >(vm, SqDppSticker::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppSticker::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpSticker::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("StickerPack"),
        Class< DpStickerPack, NoConstructor< DpStickerPack > >(vm, SqDppStickerPack::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppStickerPack::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpStickerPack::IsValid)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Message"),
        Class< DpMessage, NoConstructor< DpMessage > >(vm, SqDppMessage::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppMessage::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpMessage::IsValid)
    );
}

} // Namespace:: SqMod
