"""Data contracts and sequence packing. No corpora ship with this package."""

from elafry.data.contracts import (
    PackedBlock,
    PreferencePair,
    Turn,
    validate_preference,
    validate_turn,
)
from elafry.data.packing import (
    TokenStream,
    encode_document,
    encode_sft_document,
    load_stream,
    pack_blocks,
    save_stream,
)

__all__ = [
    "PackedBlock",
    "PreferencePair",
    "TokenStream",
    "Turn",
    "encode_document",
    "encode_sft_document",
    "load_stream",
    "pack_blocks",
    "save_stream",
    "validate_preference",
    "validate_turn",
]