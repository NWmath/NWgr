---
layout: library
title: util/tag_invoke.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  is_nothrow_tag_invocable:
    definition: std::bool_constant<is_nothrow_tag_invocable_v<CPO, Args...>>
    description: __MISSING__
  is_tag_invocable:
    definition: std::bool_constant<is_tag_invocable_v<CPO, Args...>>
    description: __MISSING__
  no_type:
    definition: char (&)[2]
    description: __MISSING__
  tag_invoke_result_t:
    definition: decltype(tag_invoke(std::declval<CPO>(), std::declval<Args>()...))
    description: __MISSING__
  tag_t:
    definition: std::remove_cvref_t<decltype(CPO)>
    description: __MISSING__
  yes_type:
    definition: char
    description: __MISSING__
---

```{index}  util/tag_invoke.hpp
```

