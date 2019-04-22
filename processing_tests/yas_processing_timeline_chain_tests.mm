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
    timeline timeline;

    track track;
    timeline.insert_track(0, track);

    std::vector<timeline::event_t> events;
    std::vector<std::tuple<track_index_t, proc::track, std::map<time::range, chaining::vector::holder<module>>>>
        relayed;

    auto chain =
        timeline.chain()
            .perform([&events, &relayed](auto const &event) {
                events.push_back(event);
                proc::timeline::relayed_event_t const &relayed_event =
                    event.template get<proc::timeline::relayed_event_t>();
                if (relayed_event.relayed.type() == track::event_type_t::inserted) {
                    proc::track::inserted_event_t const &inserted_event =
                        relayed_event.relayed.template get<proc::track::inserted_event_t>();
                    relayed.push_back(std::make_tuple(relayed_event.key, relayed_event.value, inserted_event.elements));
                }
            })
            .end();

    proc::module module{[] { return module::processors_t{}; }};
    track.push_back_module(module, {0, 1});

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type(), timeline::event_type_t::relayed);
    XCTAssertEqual(relayed.size(), 1);
    XCTAssertEqual(std::get<0>(relayed.at(0)), 0);
    XCTAssertEqual(std::get<1>(relayed.at(0)), track);
    XCTAssertEqual(std::get<2>(relayed.at(0)).begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(std::get<2>(relayed.at(0)).begin()->second.size(), 1);
    XCTAssertEqual(std::get<2>(relayed.at(0)).begin()->second.at(0), module);
}

@end
