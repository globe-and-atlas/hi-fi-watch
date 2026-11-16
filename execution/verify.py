"""Verify identity, target, and generated numeral bounds in the production bundle."""
from pathlib import Path
import json
import zipfile
import subprocess

ROOT = Path(__file__).resolve().parents[1]


def main() -> None:
    package = json.loads((ROOT / 'watchface/package.json').read_text())
    assert package['pebble']['displayName'] == 'Hi-Fi'
    assert package['pebble']['targetPlatforms'] == ['emery','chalk','gabbro']
    with zipfile.ZipFile(ROOT / f"dist/Hi-Fi-{package['version']}.pbw") as bundle:
        info = json.loads(bundle.read('appinfo.json'))
        assert info['uuid'] == package['pebble']['uuid']
        assert info['versionLabel'] == package['version']
        assert info['targetPlatforms'] == package['pebble']['targetPlatforms']
        assert 'emery/pebble-app.bin' in bundle.namelist()
        assert 'pebble-js-app.js' in bundle.namelist()
        for platform in ['emery','chalk','gabbro']:
            assert bundle.read(platform+'/pebble-app.bin') == (ROOT / 'watchface/build' / platform / 'pebble-app.bin').read_bytes()
    assert not json.loads((ROOT/'.tmp/last-build.json').read_text())['fixture']
    nm=Path.home()/'Library/Application Support/Pebble SDK/SDKs/current/toolchain/arm-none-eabi/bin/arm-none-eabi-nm'
    for platform in ['emery','chalk','gabbro']:
        symbols=subprocess.run([str(nm),str(ROOT/f'watchface/build/{platform}/pebble-app.elf')],check=True,capture_output=True,text=True).stdout
        for field in ['f_hour','f_minute','f_second','f_battery','r_proof']:assert field not in symbols, 'Fixture state in production: '+field
    print('PASS: Hi-Fi production bundle for Emery, Chalk and Gabbro')


if __name__ == '__main__':
    main()
