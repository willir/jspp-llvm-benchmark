use std::convert::TryInto;
use std::slice;

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct Arr {
    pub data: *const u8,
    pub len: i32,
}

#[no_mangle]
pub extern "C" fn murmur_rust(key: *const Arr, seed: u32) -> u32 {
    let key = unsafe { slice::from_raw_parts((*key).data, (*key).len as usize) };

    let mut h = seed;

    if key.len() > 3 {
        let len = (key.len() >> 2) << 2;
        for i in (0..len).step_by(4) {
            let mut k = key[i + 3] as u32;

            k <<= 8;
            k |= key[i + 2] as u32;

            k <<= 8;
            k |= key[i + 1] as u32;

            k <<= 8;
            k |= key[i] as u32;

            k = k.wrapping_mul(0xcc9e2d51);
            k = k.rotate_left(15);
            k = k.wrapping_mul(0x1b873593);
            h ^= k;
            h = h.rotate_left(13);
            h = h.wrapping_mul(5).wrapping_add(0xe6546b64);
        }
    }
    if (key.len() & 3) != 0 {
        let mut i = key.len() & 3;
        let last_even_i = (key.len() >> 2) << 2;
        let mut k = 0u32;
        loop {
            k <<= 8;
            k |= key[last_even_i + i - 1] as u32;
            i -= 1;
            if i == 0 {
                break;
            }
        }

        k = k.wrapping_mul(0xcc9e2d51);
        k = k.rotate_left(15);
        k = k.wrapping_mul(0x1b873593);
        h ^= k;
    }
    h ^= key.len() as u32;
    h ^= h >> 16;
    h = h.wrapping_mul(0x85ebca6b);
    h ^= h >> 13;
    h = h.wrapping_mul(0xc2b2ae35);
    h ^= h >> 16;

    h
}

#[no_mangle]
pub extern "C" fn murmur_rust2(key: *const Arr, seed: u32) -> u32 {
    let key = unsafe { slice::from_raw_parts((*key).data, (*key).len as usize) };

    let mut h = seed;

    let mut chunks_it = key.chunks_exact(4);
    for chunk in &mut chunks_it {
        h = update_from_mid_chunk(chunk, h);
    }
    h = update_from_last_chunk(chunks_it.remainder(), h);

    h ^= key.len() as u32;
    h ^= h >> 16;
    h = h.wrapping_mul(0x85ebca6b);
    h ^= h >> 13;
    h = h.wrapping_mul(0xc2b2ae35);
    h ^= h >> 16;

    h
}

fn update_from_mid_chunk(chunk: &[u8], mut h: u32) -> u32 {
    h ^= buf_to_k(chunk.try_into().expect("4-element slice expected"));

    h = h.rotate_left(13);
    h = h.wrapping_mul(5).wrapping_add(0xe6546b64);

    h
}

fn update_from_last_chunk(chunk: &[u8], h: u32) -> u32 {
    let mut buf = [0, 0, 0, 0];
    buf[..chunk.len()].copy_from_slice(chunk);

    h ^ buf_to_k(buf)
}

fn buf_to_k(chunk: [u8; 4]) -> u32 {
    u32::from_le_bytes(chunk)
        .wrapping_mul(0xcc9e2d51)
        .rotate_left(15)
        .wrapping_mul(0x1b873593)
}
