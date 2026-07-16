/**
 * @file tag.hpp
 * @author SeveraTheDuck
 * @brief Base tag for eager pipeline terminators.
 */
#pragma once

namespace typus::tag {

/**
 * @brief Base class tag used to identify pipeline terminators.
 *
 * Any structure that inherits from `ValueTerminator` satisfies the
 * `model::Terminator` concept. When an anchored pipeline is piped (`operator|`)
 * into a type carrying this tag, it triggers the eager evaluation of the AST
 * and materializes a final value.
 */
struct ValueTerminator {};

}  // namespace typus::tag
