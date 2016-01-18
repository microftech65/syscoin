#include "test/test_syscoin_services.h"
#include "utiltime.h"
#include "rpcserver.h"
#include <boost/test/unit_test.hpp>
BOOST_FIXTURE_TEST_SUITE (syscoin_escrow_tests, BasicSyscoinTestingSetup)

BOOST_AUTO_TEST_CASE (generate_escrow_release)
{
	UniValue r;
	GenerateBlocks(200);
	GenerateBlocks(200, "node2");
	GenerateBlocks(200, "node3");
	AliasNew("node1", "buyeralias", "changeddata1");
	AliasNew("node2", "selleralias", "changeddata2");
	AliasNew("node3", "arbiteralias", "changeddata3");
	string qty = "3";
	string message = "paymentmessage";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "0.05", "description", "USD");
	string guid = EscrowNew("node1", offerguid, qty, message, "arbiteralias");
	EscrowRelease("node1", guid);	
	EscrowClaimRelease("node2", guid);
}
BOOST_AUTO_TEST_CASE (generate_escrow_big)
{
	UniValue r;
	// 255 bytes long
	string goodname1 = "SgsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsDfdfdd";
	string goodname2 = "TgsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsDfdfdd";
	string goodname3 = "UgsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsfDsdsdsdsfsfsdsfsdsfdsfsdsfdsfsdsfsdSfsdfdfsdsfSfsdfdfsdsDfdfdd";
	// 1023 bytes long
	string gooddata = "agdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfssdsfsdfsdfsdfsdfsdsdfdfsdfsdfsdfsd";	
	// 1024 bytes long
	string baddata = "azsdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfasdfasdfsadfsadassdsfsdfsdfsdfsdfsdsdfssdsfsdfsdfsdfsdfsdsdfdfsdfsdfsdfsd";	
				
	AliasNew("node1", goodname1, "changeddata1");
	AliasNew("node2", goodname2, "changeddata2");
	AliasNew("node3", goodname3, "changeddata3");
	string qty = "3";

	string offerguid = OfferNew("node2", goodname2, "category", "title", "100", "0.05", "description", "USD");
	// payment message too long
	BOOST_CHECK_THROW(r = CallRPC("node1", "escrownew " + offerguid + " " + qty + " " + baddata + " " + goodname3), runtime_error);
	string guid = EscrowNew("node1", offerguid, qty, gooddata, goodname3);
	EscrowRelease("node1", guid);	
	EscrowClaimRelease("node2", guid);
}
BOOST_AUTO_TEST_CASE (generate_escrowrefund_seller)
{
	string qty = "4";
	string message = "paymentmessage";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "1.22", "description", "CAD");
	string guid = EscrowNew("node1", offerguid, qty, message, "arbiteralias");
	EscrowRefund("node2", guid);
	EscrowClaimRefund("node1", guid, false);
}
BOOST_AUTO_TEST_CASE (generate_escrowrefund_arbiter)
{
	string qty = "5";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "0.25", "description", "EUR");
	string message = "paymentmessage";
	string guid = EscrowNew("node1", offerguid, qty, message, "arbiteralias");
	EscrowRefund("node3", guid);
	EscrowClaimRefund("node1", guid, true);
}
BOOST_AUTO_TEST_CASE (generate_escrowrefund_invalid)
{
	string qty = "2";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "1.45", "description", "EUR");
	string guid = EscrowNew("node1", offerguid, qty, "message", "arbiteralias");
	// try to claim refund even if not refunded
	BOOST_CHECK_THROW(CallRPC("node2", "escrowclaimrefund " + guid), runtime_error);
	// buyer cant refund to himself
	BOOST_CHECK_THROW(CallRPC("node1", "escrowrefund " + guid), runtime_error);
	EscrowRefund("node2", guid);
	// try to release already refunded escrow
	BOOST_CHECK_THROW(CallRPC("node1", "escrowrelease " + guid), runtime_error);
	// cant refund already refunded escrow
	BOOST_CHECK_THROW(CallRPC("node2", "escrowrefund " + guid), runtime_error);
	// noone other than buyer can claim release
	BOOST_CHECK_THROW(CallRPC("node3", "escrowclaimrefund " + guid), runtime_error);
	BOOST_CHECK_THROW(CallRPC("node2", "escrowclaimrefund " + guid), runtime_error);
	EscrowClaimRefund("node1", guid, false);
	// cant inititate another refund after claimed already
	BOOST_CHECK_THROW(CallRPC("node1", "escrowrefund " + guid), runtime_error);
}
BOOST_AUTO_TEST_CASE (generate_escrowrelease_invalid)
{
	string qty = "4";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "1.45", "description", "SYS");
	string guid = EscrowNew("node1", offerguid, qty, "message", "arbiteralias");
	// try to claim release even if not released
	BOOST_CHECK_THROW(CallRPC("node2", "escrowclaimrelease " + guid), runtime_error);
	// seller cant release buyers funds
	BOOST_CHECK_THROW(CallRPC("node2", "escrowrelease " + guid), runtime_error);
	EscrowRelease("node1", guid);
	// try to refund already released escrow
	BOOST_CHECK_THROW(CallRPC("node2", "escrowrefund " + guid), runtime_error);
	// cant release already released escrow
	BOOST_CHECK_THROW(CallRPC("node1", "escrowrelease " + guid), runtime_error);
	// noone other than seller can claim release
	BOOST_CHECK_THROW(CallRPC("node3", "escrowclaimrelease " + guid), runtime_error);
	BOOST_CHECK_THROW(CallRPC("node1", "escrowclaimrelease " + guid), runtime_error);
	EscrowClaimRelease("node2", guid);
	// cant inititate another release after claimed already
	BOOST_CHECK_THROW(CallRPC("node1", "escrowrelease " + guid), runtime_error);
}
BOOST_AUTO_TEST_CASE (generate_escrowrelease_arbiter)
{
	UniValue r;
	string qty = "1";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "0.05", "description", "GBP");
	string guid = EscrowNew("node1", offerguid, qty, "message", "arbiteralias");
	EscrowRelease("node3", guid);
	BOOST_CHECK_NO_THROW(r = CallRPC("node1", "escrowinfo " + guid));
	CAmount escrowfee = AmountFromValue(find_value(r.get_obj(), "sysfee"));
	// get buyer balance (ensure he gets no fees back, since arbiter stepped in and released)
	BOOST_CHECK_NO_THROW(r = CallRPC("node1", "getinfo"));
	CAmount balanceBefore = AmountFromValue(find_value(r.get_obj(), "balance"));

	EscrowClaimRelease("node2", guid);

	// get buyer balance after release
	BOOST_CHECK_NO_THROW(r = CallRPC("node1", "getinfo"));
	// 10 mined block subsidy
	balanceBefore += 10*8.25*COIN;
	CAmount balanceAfter = AmountFromValue(find_value(r.get_obj(), "balance"));
	BOOST_CHECK_EQUAL(balanceBefore, balanceAfter);

}
BOOST_AUTO_TEST_CASE (generate_escrow_linked_release)
{
	GenerateBlocks(200);
	GenerateBlocks(200, "node2");
	GenerateBlocks(200, "node3");
	string qty = "3";
	string message = "paymentmessage";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "0.04", "description", "EUR");
	string commission = "10";
	string description = "newdescription";
	// offer should be set to exclusive mode by default so linking isn't allowed
	BOOST_CHECK_THROW(CallRPC("node3", "offerlink " + offerguid + " " + commission + " " + description), runtime_error);
	offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "0.04", "description", "EUR", "", false);
	string offerlinkguid = OfferLink("node3", offerguid, commission, description);
	string guid = EscrowNew("node1", offerlinkguid, qty, message, "arbiteralias");
	EscrowRelease("node1", guid);
	// reseller cant claim escrow, seller must claim it
	BOOST_CHECK_THROW(CallRPC("node3", "escrowclaimrelease " + guid), runtime_error);
	EscrowClaimReleaseLink("node2", guid, "node3");
}
BOOST_AUTO_TEST_CASE (generate_escrow_linked_release_with_peg_update)
{
	GenerateBlocks(100);
	GenerateBlocks(100, "node2");
	GenerateBlocks(100, "node3");
	string qty = "3";
	string message = "paymentmessage";
	string offerguid = OfferNew("node2", "selleralias", "category", "title", "100", "0.05", "description", "EUR", "", false);
	string commission = "3";
	string description = "newdescription";
	string offerlinkguid = OfferLink("node3", offerguid, commission, description);
	string guid = EscrowNew("node1", offerlinkguid, qty, message, "arbiteralias");
	EscrowRelease("node1", guid);
	// update the EUR peg twice before claiming escrow
	string data = "{QuratesQu:[{QucurrencyQu:QuUSDQu,QurateQu:2690.1,QuprecisionQu:2},{QucurrencyQu:QuEURQu,QurateQu:269.2,QuprecisionQu:2},{QucurrencyQu:QuGBPQu,QurateQu:2697.3,QuprecisionQu:2},{QucurrencyQu:QuCADQu,QurateQu:2698.0,QuprecisionQu:2},{QucurrencyQu:QuBTCQu,QurateQu:100000.0,QuprecisionQu:8},{QucurrencyQu:QuSYSQu,QurateQu:1.0,QuprecisionQu:2}]}";
	BOOST_CHECK_NO_THROW(CallRPC("node1", "aliasupdate SYS_RATES " + data));
	GenerateBlocks(100);
	GenerateBlocks(100, "node2");
	GenerateBlocks(100, "node3");
	data = "{QuratesQu:[{QucurrencyQu:QuUSDQu,QurateQu:2690.1,QuprecisionQu:2},{QucurrencyQu:QuEURQu,QurateQu:218.2,QuprecisionQu:2},{QucurrencyQu:QuGBPQu,QurateQu:2697.3,QuprecisionQu:2},{QucurrencyQu:QuCADQu,QurateQu:2698.0,QuprecisionQu:2},{QucurrencyQu:QuBTCQu,QurateQu:100000.0,QuprecisionQu:8},{QucurrencyQu:QuSYSQu,QurateQu:1.0,QuprecisionQu:2}]}";
	BOOST_CHECK_NO_THROW(CallRPC("node1", "aliasupdate SYS_RATES " + data));
	GenerateBlocks(100);
	GenerateBlocks(100, "node2");
	GenerateBlocks(100, "node3");
	EscrowClaimReleaseLink("node2", guid, "node3");
	// restore EUR peg
	data = "{QuratesQu:[{QucurrencyQu:QuUSDQu,QurateQu:2690.1,QuprecisionQu:2},{QucurrencyQu:QuEURQu,QurateQu:2695.2,QuprecisionQu:2},{QucurrencyQu:QuGBPQu,QurateQu:2697.3,QuprecisionQu:2},{QucurrencyQu:QuCADQu,QurateQu:2698.0,QuprecisionQu:2},{QucurrencyQu:QuBTCQu,QurateQu:100000.0,QuprecisionQu:8},{QucurrencyQu:QuSYSQu,QurateQu:1.0,QuprecisionQu:2}]}";
	BOOST_CHECK_NO_THROW(CallRPC("node1", "aliasupdate SYS_RATES " + data));
	GenerateBlocks(10);
	GenerateBlocks(10, "node2");
	GenerateBlocks(10, "node3");
}
BOOST_AUTO_TEST_SUITE_END ()