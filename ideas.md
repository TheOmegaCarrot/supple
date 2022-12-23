# What is this document?

This is a list of ideas that may or may not be worth implementing.

---

## fmtflags

Add an overload to `to_stream` that takes an additional `std::ios::fmtflags` parameter,
to be applied only for the call to to_stream.

Likewise, add the same functionality to `stream_adapter`.
