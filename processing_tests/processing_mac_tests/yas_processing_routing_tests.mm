//
//  yas_processing_routing_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;
using namespace yas::processing;

@interface yas_processing_routing_tests : XCTestCase

@end

@implementation yas_processing_routing_tests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)test_create {
    routing routing;

    XCTAssertTrue(routing);
    XCTAssertEqual(routing.track_count(), 0);
}

- (void)test_create_null {
    routing routing = nullptr;

    XCTAssertFalse(routing);
}

- (void)test_track {
    routing routing;

    track track1;
    routing.insert_track(1, track1);

    track track2;
    routing.insert_track(2, track2);

    track trackMinus1;
    routing.insert_track(-1, trackMinus1);

    XCTAssertEqual(routing.track_count(), 3);

    XCTAssertTrue(routing.has_track(1));
    XCTAssertTrue(routing.has_track(2));
    XCTAssertTrue(routing.has_track(-1));

    XCTAssertTrue(routing.track(1) == track1);
    XCTAssertTrue(routing.track(2) == track2);
    XCTAssertTrue(routing.track(-1) == trackMinus1);

    XCTAssertFalse(routing.has_track(0));
    XCTAssertFalse(routing.track(0));
}

@end
