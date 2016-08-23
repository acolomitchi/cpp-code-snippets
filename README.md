# cpp-code-snippets
Space to contain - maybe disparate - examples of obscure C++ syntax and/or usecases

1. <tt>aliasing-templates_nested-templates_and_consuming-them.cpp</tt> - aliasing 
  types is simple
    
    using alias=the_name_of_the_type; // perhaps one needs
    using alias=typename nesting_struct::the_name_of_the_type;
   
  but how do you do it when your target for alias is a template?
  Here's how.

