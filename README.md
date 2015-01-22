CircaText
=========

Ein österreichisches Pebble-Watchface das die ungefähre Zeit in natürlicher Sprache anzeigt.

Pebble App Store: https://apps.getpebble.com/applications/543eb5b1ffbc4e01d400001d

Info: http://astifter.github.io/CircaText

Probleme? https://github.com/astifter/CircaText/issues

Fragen? circatext@googlegroups.com

Interesse? https://github.com/astifter/CircaText

How to Release
--------------

1. Increment version number in appinfo.json.
1. Make sure everything works by executing ./build_test_install.sh in the root folder, this:
   - Builds the watchface.
   - Runs the tests.
   - Installs the watchface.
2. Make sure configuration works as expected.
3. Make sure everything is on GitHub.
4. Rename build binary from build/CircaText.pbw to build/CircaText.\<major\>.\<minor\>.pbw.
5. Create GitHub release, upload renamed binary.
6. Create Pebble App Store release:
   - Make sure the release notes are of format:
     V\<major\>.\<minor\> \<description\> (\<8-digit SHA1 of GitHub release\>)
