
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import org.mockito.junit.jupiter.MockitoExtension;

import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.repository.BrandRepository;
import com.example.masya.sportstore.service.impl.BrandServiceImpl;

@ExtendWith(MockitoExtension.class)
class BrandServiceTest {

    @Mock
    private BrandRepository brandRepository;

    @InjectMocks
    private BrandServiceImpl brandService;

    @Test
    void testFindAllBrands() {
        // 1. ПОДГОТОВКА
        Brand nike = new Brand();
        nike.setId(1L);
        nike.setName("Nike");
        nike.setSince(1964);
        nike.setRating(5);

        Brand adidas = new Brand();
        adidas.setId(2L);
        adidas.setName("Adidas");
        adidas.setSince(1949);
        adidas.setRating(4);

        // 2. НАСТРОЙКА
        List<Brand> expectedBrands = Arrays.asList(nike, adidas);
        when(brandRepository.findAll()).thenReturn(expectedBrands);

        // 3. ВЫПОЛНЕНИЕ
        List<Brand> actualBrands = brandService.findAll();

        // 4. ПРОВЕРКА
        assertEquals(2, actualBrands.size());
        assertEquals("Nike", actualBrands.get(0).getName());
        assertEquals(1964, actualBrands.get(0).getSince());
        assertEquals(5, actualBrands.get(0).getRating());
        assertEquals("Adidas", actualBrands.get(1).getName());
        assertEquals(1949, actualBrands.get(1).getSince());
        assertEquals(4, actualBrands.get(1).getRating());
        verify(brandRepository, times(1)).findAll();

        System.out.println("✅ Тест 1 пройден: findAll() возвращает все бренды");
    }

    @Test
    void testFindBrandByName() {
        // 1. ПОДГОТОВКА
        Brand puma = new Brand();
        puma.setId(3L);
        puma.setName("Puma");
        puma.setSince(1948);
        puma.setRating(4);

        // 2. НАСТРОЙКА
        when(brandRepository.findByName("Puma"))
                .thenReturn(java.util.Optional.of(puma));

        // 3. ВЫПОЛНЕНИЕ
        java.util.Optional<Brand> foundBrand = brandService.findByName("Puma");

        // 4. ПРОВЕРКА
        assertTrue(foundBrand.isPresent());
        Brand actualBrand = foundBrand.get();
        assertEquals("Puma", actualBrand.getName());
        assertEquals(1948, actualBrand.getSince());
        assertEquals(4, actualBrand.getRating());
        verify(brandRepository, times(1)).findByName("Puma");

        System.out.println("✅ Тест 2 пройден: findByName() находит бренд по имени");
    }

    @Test
    void testFindBrandsByRating() {
        // 1. ПОДГОТОВКА
        Brand nike = new Brand();
        nike.setId(1L);
        nike.setName("Nike");
        nike.setRating(5);

        Brand adidas = new Brand();
        adidas.setId(2L);
        adidas.setName("Adidas");
        adidas.setRating(4);

        // 2. НАСТРОЙКА
        when(brandRepository.findByRatingGreaterThanEqual(4))
                .thenReturn(Arrays.asList(nike, adidas));

        // 3. ВЫПОЛНЕНИЕ
        List<Brand> highRatedBrands = brandService.findByRatingGreaterThanEqual(4);

        // 4. ПРОВЕРКА
        assertEquals(2, highRatedBrands.size());
        assertEquals("Nike", highRatedBrands.get(0).getName());
        assertEquals("Adidas", highRatedBrands.get(1).getName());
        verify(brandRepository, times(1)).findByRatingGreaterThanEqual(4);

        System.out.println("✅ Тест 3 пройден: findByRatingGreaterThanEqual() находит бренды по рейтингу");
    }
}