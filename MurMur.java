import java.io.FileInputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.Exception;


class MurMur { 
    // from 64-bit linear congruential generator
  public static final long NULL_HASHCODE = 2862933555777941757L;

  // Constants for 32 bit variant
  private static final int C1_32 = 0xcc9e2d51;
  private static final int C2_32 = 0x1b873593;
  private static final int R1_32 = 15;
  private static final int R2_32 = 13;
  private static final int M_32 = 5;
  private static final int N_32 = 0xe6546b64;

  // Constants for 128 bit variant
  private static final long C1 = 0x87c37b91114253d5L;
  private static final long C2 = 0x4cf5ad432745937fL;
  private static final int R1 = 31;
  private static final int R2 = 27;
  private static final int R3 = 33;
  private static final int M = 5;
  private static final int N1 = 0x52dce729;
  private static final int N2 = 0x38495ab5;

  public static final int DEFAULT_SEED = 104729;

  /**
   * Murmur3 32-bit variant.
   *
   * @param data   - input byte array
   * @param offset - offset of data
   * @param length - length of array
   * @param seed   - seed. (default 0)
   * @return - hashcode
   */
  public static int hash32(byte[] data, int offset, int length, int seed) {
    int hash = seed;
    final int nblocks = length >> 2;

    // body
    for (int i = 0; i < nblocks; i++) {
      int i_4 = i << 2;
      int k = (data[offset + i_4] & 0xff)
          | ((data[offset + i_4 + 1] & 0xff) << 8)
          | ((data[offset + i_4 + 2] & 0xff) << 16)
          | ((data[offset + i_4 + 3] & 0xff) << 24);

      hash = mix32(k, hash);
    }

    // tail
    int idx = nblocks << 2;
    int k1 = 0;
    switch (length - idx) {
      case 3:
        k1 |= data[offset + idx + 2] << 16;
      case 2:
        k1 |= data[offset + idx + 1] << 8;
      case 1:
        k1 |= data[offset + idx];

        // mix functions
        k1 *= C1_32;
        k1 = Integer.rotateLeft(k1, R1_32);
        k1 *= C2_32;
        hash ^= k1;
    }

    return fmix32(length, hash);
  }

  private static int mix32(int k, int hash) {
    k *= C1_32;
    k = Integer.rotateLeft(k, R1_32);
    k *= C2_32;
    hash ^= k;
    return Integer.rotateLeft(hash, R2_32) * M_32 + N_32;
  }

  private static int fmix32(int length, int hash) {
    hash ^= length;
    hash ^= (hash >>> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >>> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >>> 16);

    return hash;
  }
  
  static final int DATA_SIZE = 256 * 1024 + 3;
  static final int ITERATIONS = 1000;

  public static void main(String [] args) throws FileNotFoundException, IOException, Exception {
    String fileName = "/home/willir/tmp/rand";
    File file = new File(fileName);
    
    byte[] data = new byte[DATA_SIZE];

    FileInputStream fileInputStream = new FileInputStream(file);
    int read = fileInputStream.read(data);
    if (read != data.length) {
      throw new Exception("Haven't read the whole file");
    }
  
    int res = 0;
    long startTime = System.nanoTime();
    for (int i = 0; i < ITERATIONS; ++i) {
      res += hash32(data, 0, data.length, 1847734911);
    }
    long stopTime = System.nanoTime();
    System.out.println("res: " + res);
    System.out.println("time: " + ((double)(stopTime - startTime)) / ITERATIONS / 1000 + "ms.");
  }
}
