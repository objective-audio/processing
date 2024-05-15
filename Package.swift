// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "audio-processing",
    platforms: [.macOS(.v14), .iOS(.v17), .macCatalyst(.v17)],
    products: [
        .library(
            name: "audio-processing",
            targets: ["audio-processing"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/objective-audio/audio_engine.git", branch: "master"),
    ],
    targets: [
        .target(
            name: "audio-processing",
            dependencies: [
                .product(name: "audio", package: "audio_engine")
            ],
            cSettings: [
                .unsafeFlags(["-fmodules"]),
            ]
        ),
        .testTarget(
            name: "audio-processing-tests",
            dependencies: [
                "audio-processing",
            ],
            cxxSettings: [
                .unsafeFlags(["-fcxx-modules"]),
            ]
        ),
    ],
    cLanguageStandard: .gnu18,
    cxxLanguageStandard: .gnucxx2b
)
