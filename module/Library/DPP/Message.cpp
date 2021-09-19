// ------------------------------------------------------------------------------------------------
#include "Library/DPP/Message.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqDppSelectOption, _SC("SqDppSelectOption"))
SQMOD_DECL_TYPENAME(SqDppSelectOptions, _SC("SqDppSelectOptions"))
SQMOD_DECL_TYPENAME(SqDppComponent, _SC("SqDppComponent"))
SQMOD_DECL_TYPENAME(SqDppComponents, _SC("SqDppComponents"))
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
    Register_DPP_VectorProxy< dpp::select_option, DpSelectOption, SqDppSelectOptions >(vm, ns, _SC("SelectOptions"));
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("Component"),
        Class< DpComponent, NoConstructor< DpComponent > >(vm, SqDppComponent::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppComponent::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpComponent::IsValid)
        .Prop(_SC("JSON"), &DpComponent::BuildJSON)
        .Prop(_SC("Type"), &DpComponent::GetType, &DpComponent::SetType)
        .Prop(_SC("Label"), &DpComponent::GetLabel, &DpComponent::SetLabel)
        .Prop(_SC("Style"), &DpComponent::GetStyle, &DpComponent::SetStyle)
        .Prop(_SC("CustomID"), &DpComponent::GetCustomID, &DpComponent::SetCustomID)
        .Prop(_SC("URL"), &DpComponent::GetURL, &DpComponent::SetURL)
        .Prop(_SC("Placeholder"), &DpComponent::GetPlaceholder, &DpComponent::SetPlaceholder)
        .Prop(_SC("MinValues"), &DpComponent::GetMinValues, &DpComponent::SetMinValues)
        .Prop(_SC("MaxValues"), &DpComponent::GetMaxValues, &DpComponent::SetMaxValues)
        .Prop(_SC("Disabled"), &DpComponent::IsDisabled, &DpComponent::SetDisabled)
        .Prop(_SC("IsAnimated"), &DpComponent::IsAnimated, &DpComponent::SetAnimated)
        .Prop(_SC("EmojiName"), &DpComponent::GetEmojiName, &DpComponent::SetEmojiName)
        .Prop(_SC("EmojiID"), &DpComponent::GetEmojiID, &DpComponent::SetEmojiID)
        // Member Methods
        .FmtFunc(_SC("SetLabel"), &DpComponent::ApplyLabel)
        .FmtFunc(_SC("SetCustomID"), &DpComponent::ApplyCustomID)
        .FmtFunc(_SC("SetURL"), &DpComponent::ApplyURL)
        .FmtFunc(_SC("SetPlaceholder"), &DpComponent::ApplyPlaceholder)
        .FmtFunc(_SC("SetDisabled"), &DpComponent::SetDisabled)
        .FmtFunc(_SC("SetAnimated"), &DpComponent::SetAnimated)
        .FmtFunc(_SC("SetEmojiName"), &DpComponent::ApplyEmojiName)
        .FmtFunc(_SC("GetComponents"), &DpComponent::GetComponents)
        .FmtFunc(_SC("GetOptions"), &DpComponent::GetOptions)
        .FmtFunc(_SC("AddComponent"), &DpComponent::AddComponent)
        .FmtFunc(_SC("AddOption"), &DpComponent::AddSelectOption)
    );
    // --------------------------------------------------------------------------------------------
    Register_DPP_VectorProxy< dpp::component, DpComponent, SqDppComponent >(vm, ns, _SC("Components"));
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedFooter"),
        Class< DpEmbedFooter, NoConstructor< DpEmbedFooter > >(vm, SqDppEmbedFooter::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedFooter::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedFooter::IsValid)
        .Prop(_SC("Text"), &DpEmbedFooter::GetText, &DpEmbedFooter::SetText)
        .Prop(_SC("Icon"), &DpEmbedFooter::GetIconURL, &DpEmbedFooter::SetIconURL)
        .Prop(_SC("Proxy"), &DpEmbedFooter::GetProxyURL, &DpEmbedFooter::SetProxyURL)
        // Member Methods
        .FmtFunc(_SC("SetText"), &DpEmbedFooter::ApplyText)
        .FmtFunc(_SC("SetIcon"), &DpEmbedFooter::ApplyIconURL)
        .FmtFunc(_SC("SetProxy"), &DpEmbedFooter::ApplyProxyURL)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedImage"),
        Class< DpEmbedImage, NoConstructor< DpEmbedImage > >(vm, SqDppEmbedImage::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedImage::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedImage::IsValid)
        .Prop(_SC("URL"), &DpEmbedImage::GetURL, &DpEmbedImage::SetURL)
        .Prop(_SC("Proxy"), &DpEmbedImage::GetProxyURL, &DpEmbedImage::SetProxyURL)
        .Prop(_SC("Height"), &DpEmbedImage::GetHeight, &DpEmbedImage::SetHeight)
        .Prop(_SC("Width"), &DpEmbedImage::GetWidth, &DpEmbedImage::SetWidth)
        // Member Methods
        .FmtFunc(_SC("SetURL"), &DpEmbedImage::ApplyURL)
        .FmtFunc(_SC("SetProxy"), &DpEmbedImage::ApplyProxyURL)
        .FmtFunc(_SC("SetHeight"), &DpEmbedImage::ApplyHeight)
        .FmtFunc(_SC("SetWidth"), &DpEmbedImage::ApplyWidth)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedProvider"),
        Class< DpEmbedProvider, NoConstructor< DpEmbedProvider > >(vm, SqDppEmbedProvider::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedProvider::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedProvider::IsValid)
        .Prop(_SC("Name"), &DpEmbedProvider::GetName, &DpEmbedProvider::SetName)
        .Prop(_SC("URL"), &DpEmbedProvider::GetURL, &DpEmbedProvider::SetURL)
        // Member Methods
        .FmtFunc(_SC("SetName"), &DpEmbedProvider::ApplyName)
        .FmtFunc(_SC("SetURL"), &DpEmbedProvider::ApplyURL)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedAuthor"),
        Class< DpEmbedAuthor, NoConstructor< DpEmbedAuthor > >(vm, SqDppEmbedAuthor::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedAuthor::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedAuthor::IsValid)
        .Prop(_SC("Name"), &DpEmbedAuthor::GetName, &DpEmbedAuthor::SetName)
        .Prop(_SC("URL"), &DpEmbedAuthor::GetURL, &DpEmbedAuthor::SetURL)
        .Prop(_SC("Icon"), &DpEmbedAuthor::GetIconURL, &DpEmbedAuthor::SetIconURL)
        .Prop(_SC("ProxyIcon"), &DpEmbedAuthor::GetProxyIconURL, &DpEmbedAuthor::SetProxyIconURL)
        // Member Methods
        .FmtFunc(_SC("SetName"), &DpEmbedAuthor::ApplyName)
        .FmtFunc(_SC("SetURL"), &DpEmbedAuthor::ApplyURL)
        .FmtFunc(_SC("SetIcon"), &DpEmbedAuthor::ApplyIconURL)
        .FmtFunc(_SC("SetProxyIcon"), &DpEmbedAuthor::ApplyProxyIconURL)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("EmbedField"),
        Class< DpEmbedField, NoConstructor< DpEmbedField > >(vm, SqDppEmbedField::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqDppEmbedField::Fn)
        // Member Properties
        .Prop(_SC("Valid"), &DpEmbedField::IsValid)
        .Prop(_SC("Name"), &DpEmbedField::GetName, &DpEmbedField::SetName)
        .Prop(_SC("Value"), &DpEmbedField::GetValue, &DpEmbedField::SetValue)
        .Prop(_SC("Inline"), &DpEmbedField::IsInline, &DpEmbedField::SetInline)
        // Member Methods
        .FmtFunc(_SC("SetName"), &DpEmbedField::ApplyName)
        .FmtFunc(_SC("SetValue"), &DpEmbedField::ApplyValue)
        .FmtFunc(_SC("SetInline"), &DpEmbedField::SetInline)
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
