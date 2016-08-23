/*
 * dummy_tests.cpp
 *
 *  Created on: 23 Aug 2016
 *      Author: acolomitchi
 */


#include <boost/optional.hpp>
#include <memory>
#include <type_traits>

template <typename T, template <typename> class null_handler>
class GenericHandlerBase /* : public IHandler */ {
  // makes use of *ANY* null_handler and is happy, thanks for asking

  // But... specialized classes of the GenericHandler would tie the
  // the null_handler based on the T type.

};

// Default null_assigner assumes the value is required
template <typename T> struct req_null_assigner {
  void operator()(T& dest) {
    throw std::runtime_error("Cannot assign null to a required value");
  }
};

// The null_assigner for boost:optional values handles
// the assignment in the boost::optional specific way
template <typename T> struct opt_null_assigner {
  void operator()(boost::optional<T>& dest) {
    dest=boost::none;
  }
};


// The null_assigner for std::shared_ptr values handles
// the assignment... well, you guessed... in the std::shared_ptr specific way
template <typename T> struct shptr_null_assigner {
  void operator()(std::shared_ptr<T>& dest) {
    dest.reset();
  }
};

// testing a "templated typedef" syntax in the namespace context
// **OBSCURE SYNTAX** note that the `req_null_assigner` need NOT to be
// preceded by `template` and
// NEED NOT to be fully-namespace qualified WHEN IN THE SAME NAMESPACE
template <typename U> using default_null_assigner=req_null_assigner<U>;
default_null_assigner<int> alpha; // works fine, no compilation error

// A "null_assigner deducer" implementing the "tie" between
// the T parameter and the null_assigner of the kind that's appropriate
// for the "assignment handling"
// The default will return the "required" behavior
template <typename T> struct null_assigner_deducer {
  // **OBSCURE SYNTAX**: note that the `template req_null_assigner`
  // MUST be prefixed/fully-qualified with the namespace
  template <typename U> using op_type=typename ::template req_null_assigner<U>;
};
// specialization for boost::optional will return the op_null_assigner
template <typename T>
struct null_assigner_deducer<boost::optional<T>> {
  // **OBSCURE SYNTAX**: note that the `template req_null_assigner`
  // MUST be prefixed/fully-qualified with the namespace
  template <typename U> using op_type=typename ::template opt_null_assigner<U>;
};
// and, of course, the specialization fot std::shared_ptr will return the shptr_null_assigner
template <typename T>
struct null_assigner_deducer<std::shared_ptr<T>> {
  // **OBSCURE SYNTAX**: note that the `template req_null_assigner`
  // MUST be prefixed/fully-qualified with the namespace
  template <typename U> using op_type=typename ::template shptr_null_assigner<U>;
};

// testing that instances of null_assigners returned by deducer are OK
null_assigner_deducer<int>::op_type<long> beta1; // compiles fine, no syntax error
null_assigner_deducer<boost::optional<long>>::op_type<int> beta2; // compiles OK
null_assigner_deducer<std::shared_ptr<double>>::op_type<char> beta3; // fine too

static_assert(std::is_same<req_null_assigner<long>, decltype(beta1)>::value, "beta1 failed");
static_assert(std::is_same<opt_null_assigner<int>, decltype(beta2)>::value, "beta2 failed");
static_assert(std::is_same<shptr_null_assigner<char>, decltype(beta3)>::value, "beta3 failed");

// Whoo-hoo. Now I can define my "GenericAssigner" based on the
// functionality already in the GenericHandler, can't I?

// In the context of assigning the values we received in IHandler,
// we'll use a GenericAssigner which will just pick the null_handler
// based in the provided T
template <typename T>
class GenericAssigner :
    public GenericHandlerBase<
      T,
      // **OBSCURE SYNTAX** here only the `template` qualifier before the op_type
      null_assigner_deducer<T>::template op_type
    >
{
};
