import java.util.Random;
import java.lang.reflect.Field;

public class Gen extends Random {
    public void printProperties() throws Exception {
        System.out.println("Class: " + this.getClass().getName());

        Class<?> superClass = Random.class;
        for (String name : new String[]{"multiplier", "addend", "mask"}) {
            Field field = superClass.getDeclaredField(name);
            field.setAccessible(true);
            long value = field.getLong(null);
            System.out.printf("  %s = 0x%X (%d)%n", name, value, value);
        }
    }

    public static void main(String[] args) {
        final String delimiter = "################################";
        final int N = 6;
        final Integer SEED = 1;
        final boolean emulateBallyMoneyHoney = true;
        final int numRollCards = 32;
        Gen rng = new Gen();

        if (SEED != 0) {
            rng.setSeed(SEED.intValue());
        }
        try {
            // rng.printProperties();
        } catch (Exception e) {
            // e.printStackTrace();
            System.out.println("Run with java --add-opens java.base/java.util=ALL-UNNAMED -cp . Gen");
        }
        System.out.println("Generating " + N + " 48-bit values from Random:");
        System.out.println(delimiter);
        for (int i = 0; i < N; i++) {
            long first;
            long second;
            long third;

            if (i == N/2) {
                System.out.println(delimiter);
            }

            if (emulateBallyMoneyHoney) {
                first = Integer.toUnsignedLong(rng.nextInt(numRollCards));
                second  = Integer.toUnsignedLong(rng.nextInt(numRollCards));
                third  = Integer.toUnsignedLong(rng.nextInt(numRollCards));
                System.out.printf("%02X %02X %02X%n", first, second, third);
            } else {
                first = Integer.toUnsignedLong(rng.nextInt());
                second  = Integer.toUnsignedLong(rng.nextInt());
                third = Integer.toUnsignedLong(rng.nextInt());
                System.out.printf("%08X %08X %08X%n", first, second, third);
            }
        }
    }
}
