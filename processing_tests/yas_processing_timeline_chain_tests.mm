//
//  yas_processing_timeline_chain_tests.mm
//

#import <XCTest/XCTest.h>
#import <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::proc;

@interface yas_processing_timeline_chain_tests : XCTestCase

@end

@implementation yas_processing_timeline_chain_tests

- (void)setUp {
}

- (void)tearDown {
}

- (void)test_fetched {
    timeline timeline;

    track track0;
    track track1;

    timeline.insert_track(0, track0);
    timeline.insert_track(1, track1);

    std::vector<timeline::event_t> events;

    auto chain = timeline.chain().perform([&events](auto const &event) { events.push_back(event); }).sync();

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), timeline::event_type_t::fetched);
    auto iterator = events.at(0).get<timeline::fetched_event_t>().elements.begin();
    XCTAssertEqual(iterator->first, 0);
    XCTAssertEqual(iterator->second, track0);
    ++iterator;
    XCTAssertEqual(iterator->first, 1);
    XCTAssertEqual(iterator->second, track1);
}

- (void)test_inserted {
}

- (void)test_erased {
}

- (void)test_replaced {
    // ???
}

- (void)test_relayed {
}

@end
