WORKSPACE = processing.xcworkspace
SCHEME = processing_ios_tests
CONFIGURATION = Release
SDK = iphonesimulator
DESTINATION = 'platform=iOS Simulator,name=iPhone 6,OS=9.2'

test_osx:
	xcodebuild -workspace $(WORKSPACE) -scheme $(SCHEME) -configuration $(CONFIGURATION) OBJROOT=build -sdk $(SDK) clean test
