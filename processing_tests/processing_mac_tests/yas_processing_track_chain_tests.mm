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

- (void)test_fetched {
    proc::track track;

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};
    track.insert_module({0, 1}, std::move(module1));
    track.insert_module({1, 1}, std::move(module2));

    std::vector<proc::track_event_t> events;

    auto chain = track.chain().perform([&events](auto const &event) { events.push_back(event); }).sync();

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type, proc::track_event_type::fetched);
}

- (void)test_inserted {
    proc::track track;

    std::vector<proc::track_event_t> events;

    auto chain = track.chain().perform([&events](auto const &event) { events.push_back(event); }).end();

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};

    track.insert_module({0, 1}, module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type, proc::track_event_type::inserted);
    XCTAssertEqual(events.at(0).elements.size(), 1);
    XCTAssertEqual(events.at(0).elements.begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(events.at(0).elements.begin()->second, module1);

    track.insert_module({1, 1}, module2);

    XCTAssertEqual(events.size(), 2);
    XCTAssertEqual(events.at(1).type, proc::track_event_type::inserted);
    XCTAssertEqual(events.at(1).elements.size(), 1);
    XCTAssertEqual(events.at(0).elements.begin()->first, (proc::time::range{1, 1}));
    XCTAssertEqual(events.at(0).elements.begin()->second, module2);
}

- (void)test_erased {
    proc::track track;

    proc::module module1{proc::module::processors_t{}};
    proc::module module2{proc::module::processors_t{}};
    track.insert_module({0, 1}, module1);
    track.insert_module({1, 1}, module2);

    std::vector<proc::track_event_t> events;
    std::vector<std::multimap<proc::time::range, proc::module>> erased;

    auto chain = track.chain()
                     .perform([&events, &erased](auto const &event) {
                         events.push_back(event);
                         if (event.type == proc::track_event_type::erased) {
                             erased.push_back(event.elements);
                         }
                     })
                     .end();

    track.erase_module(module1);

    XCTAssertEqual(events.size(), 1);
    XCTAssertEqual(events.at(0).type, proc::track_event_type::erased);

    XCTAssertEqual(erased.size(), 1);
    XCTAssertEqual(erased.at(0).size(), 1);
    XCTAssertEqual(erased.at(0).begin()->first, (proc::time::range{0, 1}));
    XCTAssertEqual(erased.at(0).begin()->second, module1);
}

@end
