//
//  yas_processing_track_chain_tests.mm
//

#import <XCTest/XCTest.h>
#import "yas_processing.h"

using namespace yas;

@interface yas_processing_track_chain_tests : XCTestCase

@end

@implementation yas_processing_track_chain_tests

- (void)setUp {
}

- (void)tearDown {
}

- (void)test_inserted {
    proc::track track;

    std::vector<proc::track_event_t> events;

    auto chain = track.chain().perform([&events](auto const &event) { events.push_back(event); }).end();

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};

    track.insert_module({0, 1}, module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), proc::track_event_type::inserted);
    XCTAssertEqual(events.at(0).get<proc::track_inserted_event_t>().elements.size(), 1);
    XCTAssertEqual(events.at(0).get<proc::track_inserted_event_t>().elements.begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(events.at(0).get<proc::track_inserted_event_t>().elements.begin()->second, module1);

    track.insert_module({1, 1}, module2);

    XCTAssertEqual(events.size(), 2);
    XCTAssertEqual(events.at(1).type(), proc::track_event_type::inserted);
    XCTAssertEqual(events.at(1).get<proc::track_inserted_event_t>().elements.size(), 1);
    XCTAssertEqual(events.at(0).get<proc::track_inserted_event_t>().elements.begin()->first, (proc::time::range{1, 1}));
    XCTAssertEqual(events.at(0).get<proc::track_inserted_event_t>().elements.begin()->second, module2);
}

@end
