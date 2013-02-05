#include <boost/test/auto_unit_test.hpp>

#include "brain.x86.assembler.hpp"
#include <boost/range.hpp>
#include <boost/preprocessor.hpp>
#include <vector>

BOOST_AUTO_TEST_CASE( test_assembler_adc_rm_r_generation ) {
	#if 0 // Set to 1 to get an assembly dump instead of a unit test
		#define ADC( arg1, arg2, expected ) __asm adc [arg1], arg2 ;
	#else
		#define PUSH_EXPECTED( z, unused, byte ) expected_code.push_back( BOOST_PP_CAT(0x,byte) );
		#define ADC( arg1, arg2, expected ) BOOST_PP_SEQ_FOR_EACH( PUSH_EXPECTED, ~, expected ) assembler_code.adc( dword[arg1] , arg2 );
	#endif

	using namespace brain::x86;

	std::vector< unsigned char > expected_code;
	assembler assembler_code;

	ADC( eax, eax, (11)(00)     ); //004124F0 11 00            adc         dword ptr [eax],eax
	ADC( ecx, eax, (11)(01)     ); //004124F2 11 01            adc         dword ptr [ecx],eax
	ADC( edx, eax, (11)(02)     ); //004124F4 11 02            adc         dword ptr [edx],eax
	ADC( ebx, eax, (11)(03)     ); //004124F6 11 03            adc         dword ptr [ebx],eax
	ADC( esp, eax, (11)(04)(24) ); //004124F8 11 04 24         adc         dword ptr [esp],eax
	ADC( ebp, eax, (11)(45)(00) ); //004124FB 11 45 00         adc         dword ptr [ebp],eax
	ADC( esi, eax, (11)(06)     ); //004124FE 11 06            adc         dword ptr [esi],eax
	ADC( edi, eax, (11)(07)     ); //00412500 11 07            adc         dword ptr [edi],eax

	__asm adc eax, eax;
	__asm adc  ax,  ax;
	__asm adc ebx, ebx;
	__asm adc  bx,  bx;

	ADC( eax+4, eax, (11)(40)(04)     ); //00443477 11 40 04         adc         dword ptr [eax+4],eax 
	ADC( ecx+4, eax, (11)(41)(04)     ); //0044347A 11 41 04         adc         dword ptr [ecx+4],eax 
	ADC( edx+4, eax, (11)(42)(04)     ); //0044347D 11 42 04         adc         dword ptr [edx+4],eax 
	ADC( ebx+4, eax, (11)(43)(04)     ); //00443480 11 43 04         adc         dword ptr [ebx+4],eax 
	ADC( esp+4, eax, (11)(44)(24)(04) ); //00443483 11 44 24 04      adc         dword ptr [esp+4],eax 
	ADC( ebp+4, eax, (11)(45)(04)     ); //00443487 11 45 04         adc         dword ptr [ebp+4],eax 
	ADC( esi+4, eax, (11)(46)(04)     ); //0044348A 11 46 04         adc         dword ptr [esi+4],eax 
	ADC( edi+4, eax, (11)(47)(04)     ); //0044348D 11 47 04         adc         dword ptr [edi+4],eax 

	ADC( eax-4, eax, (11)(40)(FC)     );
	ADC( ecx-4, eax, (11)(41)(FC)     );
	ADC( edx-4, eax, (11)(42)(FC)     );
	ADC( ebx-4, eax, (11)(43)(FC)     );
	ADC( esp-4, eax, (11)(44)(24)(FC) );
	ADC( ebp-4, eax, (11)(45)(FC)     );
	ADC( esi-4, eax, (11)(46)(FC)     );
	ADC( edi-4, eax, (11)(47)(FC)     );
		
	ADC( eax+0x40000, eax, (11)(80)(00)(00)(04)(00)     );
	ADC( ecx+0x40000, eax, (11)(81)(00)(00)(04)(00)     );
	ADC( edx+0x40000, eax, (11)(82)(00)(00)(04)(00)     );
	ADC( ebx+0x40000, eax, (11)(83)(00)(00)(04)(00)     );
	ADC( esp+0x40000, eax, (11)(84)(24)(00)(00)(04)(00) );
	ADC( ebp+0x40000, eax, (11)(85)(00)(00)(04)(00)     );
	ADC( esi+0x40000, eax, (11)(86)(00)(00)(04)(00)     );
	ADC( edi+0x40000, eax, (11)(87)(00)(00)(04)(00)     );

	ADC( eax-0x40000, eax, (11)(80)(00)(00)(FC)(FF)     );
	ADC( ecx-0x40000, eax, (11)(81)(00)(00)(FC)(FF)     );
	ADC( edx-0x40000, eax, (11)(82)(00)(00)(FC)(FF)     );
	ADC( ebx-0x40000, eax, (11)(83)(00)(00)(FC)(FF)     );
	ADC( esp-0x40000, eax, (11)(84)(24)(00)(00)(FC)(FF) );
	ADC( ebp-0x40000, eax, (11)(85)(00)(00)(FC)(FF)     );
	ADC( esi-0x40000, eax, (11)(86)(00)(00)(FC)(FF)     );
	ADC( edi-0x40000, eax, (11)(87)(00)(00)(FC)(FF)     );

	BOOST_CHECK( *(assembler_code.code) == expected_code );

	#undef ADC
	#undef ADC_
	#ifdef PUSH_EXPECTED
	#undef PUSH_EXPECTED
	#endif
}

BOOST_AUTO_TEST_CASE( test_assembler_adc_r_rm_generation ) {
#if 0 // example disassembly generated by MSVC
	__asm adc eax, [eax]; // 00444234 13 00            adc         eax,dword ptr [eax] 
	__asm adc eax, [ecx]; // 00444236 13 01            adc         eax,dword ptr [ecx] 
	__asm adc eax, [edx]; // 00444238 13 02            adc         eax,dword ptr [edx] 
	__asm adc eax, [ebx]; // 0044423A 13 03            adc         eax,dword ptr [ebx] 
	__asm adc eax, [esp]; // 0044423C 13 04 24         adc         eax,dword ptr [esp] 
	__asm adc eax, [ebp]; // 0044423F 13 45 00         adc         eax,dword ptr [ebp] 
	__asm adc eax, [esi]; // 00444242 13 06            adc         eax,dword ptr [esi] 
	__asm adc eax, [edi]; // 00444244 13 07            adc         eax,dword ptr [edi] #endif
#endif

	const unsigned char expected_[] = {
		0x13, 0x00,
		0x13, 0x01,
		0x13, 0x02,
		0x13, 0x03,
		0x13, 0x04, 0x24,
		0x13, 0x45, 0x00,
		0x13, 0x06,
		0x13, 0x07
	};
	const std::vector< unsigned char > expected( boost::begin(expected_), boost::end(expected_) );

	using namespace brain::x86;
	assembler a; a
		.adc( eax, dword[eax] )
		.adc( eax, dword[ecx] )
		.adc( eax, dword[edx] )
		.adc( eax, dword[ebx] )
		.adc( eax, dword[esp] )
		.adc( eax, dword[ebp] )
		.adc( eax, dword[esi] )
		.adc( eax, dword[edi] )
		;

	BOOST_CHECK( *(a.code) == expected );
}

BOOST_AUTO_TEST_CASE( test_assembler_add_generation ) {
	using namespace brain::x86;
	assembler a;

#if 0 // example disassembly generated by MSVC
		__asm add [eax], eax; // 004441E4 01 00            add         dword ptr [eax],eax 
		__asm add [ecx], eax; // 004441E6 01 01            add         dword ptr [ecx],eax 
		__asm add [edx], eax; // 004441E8 01 02            add         dword ptr [edx],eax 
		__asm add [ebx], eax; // 004441EA 01 03            add         dword ptr [ebx],eax 
		__asm add [esp], eax; // 004441EC 01 04 24         add         dword ptr [esp],eax 
		__asm add [ebp], eax; // 004441EF 01 45 00         add         dword ptr [ebp],eax 
		__asm add [esi], eax; // 004441F2 01 06            add         dword ptr [esi],eax 
		__asm add [edi], eax; // 004441F4 01 07            add         dword ptr [edi],eax 
#endif

	const unsigned char expected_[] = {
		0x01, 0x00,
		0x01, 0x01,
		0x01, 0x02,
		0x01, 0x03,
		0x01, 0x04, 0x24,
		0x01, 0x45, 0x00,
		0x01, 0x06,
		0x01, 0x07
	};
	const std::vector< unsigned char > expected( boost::begin(expected_), boost::end(expected_) );
	a
		.add( dword[eax], eax )
		.add( dword[ecx], eax )
		.add( dword[edx], eax )
		.add( dword[ebx], eax )
		.add( dword[esp], eax )
		.add( dword[ebp], eax )
		.add( dword[esi], eax )
		.add( dword[edi], eax )
		;

	BOOST_CHECK( *(a.code) == expected );
}
