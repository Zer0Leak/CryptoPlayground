import java.util.Random;

public class Player {
    final Random random = new Random();  // per Player
    static final long UINT_RANGE = 0x1_0000_0000L;                              // 2^32
    final double criticalHitChance = 0.60;                                      // Only to show in UI
    final long criticalHitThreshold = (long) (UINT_RANGE * criticalHitChance);  // hold/use this

    public static void main(String[] args) {
        Player player = new Player();

        for (int i = 0; i < 5; i++) {
            long hit = player.random.nextInt() & 0xFFFF_FFFFL;  // Simulate a 32-bit unsigned int roll

            if (hit < player.criticalHitThreshold) {  // 0 ≤ roll < threshold
                System.out.printf("Critical Hit! (Roll: %08X)%n", hit);
            } else {
                System.out.printf("Normal Hit. (Roll: %08X)%n", hit);
            }
        }
    }
}
