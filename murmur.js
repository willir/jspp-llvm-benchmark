var fs = require("fs");
const BUFFER_SIZE = 256 * 1024 + 3;
const SEED = 1847734911;


function _x86Multiply(m, n) {
    //
    // Given two 32bit ints, returns the two multiplied together as a
    // 32bit int.
    //
    return ((m & 0xffff) * n) + ((((m >>> 16) * n) & 0xffff) << 16);
}


function murmur(key, seed) {
    var h = seed >>> 0;

    if (key.length > 3) {
        var len = (key.length >>> 2) << 2;
        for (var i = 0; i < len; i += 4) {
            var k = key[i + 3] >>> 0;

            k <<= 8;
            k = (k | key[i + 2] >>> 0) >>> 0;

            k <<= 8;
            k = (k | key[i + 1] >>> 0) >>> 0;

            k <<= 8;
            k = (k | key[i] >>> 0) >>> 0;

            k = Math.imul(k, 0xcc9e2d51) >>> 0;
            k = ((k << 15 >>> 0) | (k >>> 17)) >>> 0;
            k = Math.imul(k, 0x1b873593) >>> 0;

            h = (h ^ k) >>> 0;
            h = ((h << 13) | (h >>> 19)) >>> 0;
            h = (Math.imul(h, 5) + 0xe6546b64) >>> 0;
        }
    }
    if ((key.length & 3) != 0) {
        var i = key.length & 3;
        var lastEvenI = (key.length >> 2) << 2;
        var k = 0 >>> 0;
        do {
            k <<= 8;
            k |= key[lastEvenI + i - 1] >>> 0;
        } while (--i > 0);

        k = Math.imul(k, 0xcc9e2d51) >>> 0;
        k = ((k << 15) | (k >>> 17)) >>> 0;
        k = Math.imul(k, 0x1b873593) >>> 0;
        h ^= k;
    }
    h ^= key.length;
    h = (h ^ (h >>> 16)) >>> 0;
    h = Math.imul(h, 0x85ebca6b) >>> 0;
    h = (h ^ (h >>> 13)) >>> 0;
    h = Math.imul(h, 0xc2b2ae35) >>> 0;
    h = (h ^ (h >>> 16) ) >>> 0;
    return h;
}

function measureMurmur(err, bytesRead, buf) {
    const ITERATIONS = 1000;
    const NS_PER_SEC = 1e9;

    var start = process.hrtime()
    var res = 0 >>> 0;
    for (var i = 0; i < ITERATIONS; ++i) {
        res += murmur(buf, SEED);
    }
    var diff = process.hrtime(start);
    var elapsed = diff[0] * NS_PER_SEC + diff[1];
    console.log("Elapsed: " + elapsed / ITERATIONS / 1000 + "ms.");
    console.log("Res: " + res);
}

function testMurmur() {
    fs.open("/home/willir/tmp/rand", "r", (err, fd) => {
        const buf = Buffer.alloc(BUFFER_SIZE);
        fs.read(fd, buf, 0, BUFFER_SIZE, 0, measureMurmur);
    });
}

testMurmur();
