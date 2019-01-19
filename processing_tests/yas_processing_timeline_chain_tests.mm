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
    timeline timeline;

    track track;

    std::vector<timeline::event_t> events;
    std::vector<std::map<track_index_t, proc::track>> inserted;

    auto chain = timeline.chain()
                     .perform([&events, &inserted](auto const &event) {
                         events.push_back(event);
                         inserted.push_back(event.template get<proc::timeline::inserted_event_t>().elements);
                     })
                     .end();

    timeline.insert_track(0, track);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), timeline::event_type_t::inserted);
    XCTAssertEqual(inserted.size(), 1);
    XCTAssertEqual(inserted.at(0).begin()->first, 0);
    XCTAssertEqual(inserted.at(0).begin()->second, track);
}

- (void)test_erased {
    timeline timeline;

    track track;
    timeline.insert_track(0, track);

    std::vector<timeline::event_t> events;
    std::vector<std::map<track_index_t, proc::track>> erased;

    auto chain = timeline.chain()
                     .perform([&events, &erased](auto const &event) {
                         events.push_back(event);
                         erased.push_back(event.template get<proc::timeline::erased_event_t>().elements);
                     })
                     .end();

    timeline.erase_track(0);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), timeline::event_type_t::erased);
    XCTAssertEqual(erased.size(), 1);
    XCTAssertEqual(erased.at(0).begin()->first, 0);
    XCTAssertEqual(erased.at(0).begin()->second, track);
}

- (void)test_relayed {
}

@end
