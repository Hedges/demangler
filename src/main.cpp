#include <iostream>
#include <demangler/demangler.hh>

int main(int argc, char const* argv[])
{
  std::string s = "_ZZN2nn2sf40SimpleAllInOneHipcSubDomainClientManagerILm2EE27InitializeShimLibraryHolderINS_2lm11ILogServiceENS0_4cmif6client6detail19ProcessModifierImplINS7_21DefaultProxyFilterTagEEEEENS_6ResultEPNS0_23ShimLibraryObjectHolderIT_EEPKcPNS_14MemoryResourceEENUlPNS0_13SharedPointerIS5_EEE_clESN_";
  try
  {
    s = demangler::demangle(s);
    std::cout << s << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << s << std::endl << e.what() << std::endl;
  }
}
