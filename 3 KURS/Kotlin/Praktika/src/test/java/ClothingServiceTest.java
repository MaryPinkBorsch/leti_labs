

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
import com.example.masya.sportstore.entity.Category;
import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.repository.ClothingRepository;
import com.example.masya.sportstore.service.impl.ClothingServiceImpl;

@ExtendWith(MockitoExtension.class)
class ClothingServiceTest {

    @Mock
    private ClothingRepository clothingRepository;

    @InjectMocks
    private ClothingServiceImpl clothingService;

    @Test
    void testFindAllClothing() {
        // 1. ПОДГОТОВКА
        Brand nike = new Brand();
        nike.setId(1L);
        nike.setName("Nike");
        
        Category shoes = new Category();
        shoes.setId(1L);
        shoes.setCategoryName("Shoes");
        
        Clothing clothing1 = new Clothing();
        clothing1.setId(1L);
        clothing1.setModelName("Air Max 90");
        clothing1.setPrice(120);
        clothing1.setBrand(nike);
        clothing1.setCategory(shoes);
        
        Clothing clothing2 = new Clothing();
        clothing2.setId(2L);
        clothing2.setModelName("Air Force 1");
        clothing2.setPrice(110);
        clothing2.setBrand(nike);
        clothing2.setCategory(shoes);
        
        // 2. НАСТРОЙКА
        List<Clothing> expectedClothes = Arrays.asList(clothing1, clothing2);
        when(clothingRepository.findAll()).thenReturn(expectedClothes);
        
        // 3. ВЫПОЛНЕНИЕ
        List<Clothing> actualClothes = clothingService.findAll();
        
        // 4. ПРОВЕРКА
        assertEquals(2, actualClothes.size());
        assertEquals("Air Max 90", actualClothes.get(0).getModelName());
        assertEquals(120, actualClothes.get(0).getPrice());
        assertEquals("Air Force 1", actualClothes.get(1).getModelName());
        assertEquals(110, actualClothes.get(1).getPrice());
        verify(clothingRepository, times(1)).findAll();
        
        System.out.println("✅ Тест 1 пройден: findAll() возвращает все товары");
    }

    @Test
    void testFindClothingById() {
        // 1. ПОДГОТОВКА
        Brand adidas = new Brand();
        adidas.setId(2L);
        adidas.setName("Adidas");
        
        Category jacket = new Category();
        jacket.setId(2L);
        jacket.setCategoryName("Jackets");
        
        Clothing testClothing = new Clothing();
        testClothing.setId(5L);
        testClothing.setModelName("Adicolor Jacket");
        testClothing.setPrice(80);
        testClothing.setBrand(adidas);
        testClothing.setCategory(jacket);
        
        // 2. НАСТРОЙКА
        when(clothingRepository.findById(5L)).thenReturn(java.util.Optional.of(testClothing));
        
        // 3. ВЫПОЛНЕНИЕ
        java.util.Optional<Clothing> foundClothing = clothingService.findById(5L);
        
        // 4. ПРОВЕРКА
        assertTrue(foundClothing.isPresent());
        Clothing actualClothing = foundClothing.get();
        assertEquals("Adicolor Jacket", actualClothing.getModelName());
        assertEquals(80, actualClothing.getPrice());
        assertEquals("Adidas", actualClothing.getBrand().getName());
        verify(clothingRepository, times(1)).findById(5L);
        
        System.out.println("✅ Тест 2 пройден: findById() находит товар по ID");
    }

    @Test
    void testSaveNewClothing() {
        // 1. ПОДГОТОВКА
        Brand puma = new Brand();
        puma.setId(3L);
        puma.setName("Puma");
        
        Category shorts = new Category();
        shorts.setId(3L);
        shorts.setCategoryName("Shorts");
        
        Clothing newClothing = new Clothing();
        newClothing.setModelName("Essentials Shorts");
        newClothing.setPrice(40);
        newClothing.setSeason(2);
        newClothing.setBrand(puma);
        newClothing.setCategory(shorts);
        
        Clothing savedClothing = new Clothing();
        savedClothing.setId(10L);
        savedClothing.setModelName("Essentials Shorts");
        savedClothing.setPrice(40);
        savedClothing.setSeason(2);
        savedClothing.setBrand(puma);
        savedClothing.setCategory(shorts);
        
        // 2. НАСТРОЙКА
        when(clothingRepository.save(newClothing)).thenReturn(savedClothing);
        
        // 3. ВЫПОЛНЕНИЕ
        Clothing result = clothingService.save(newClothing);
        
        // 4. ПРОВЕРКА
        assertEquals(10L, result.getId());
        assertEquals("Essentials Shorts", result.getModelName());
        assertEquals(40, result.getPrice());
        assertEquals("Puma", result.getBrand().getName());
        verify(clothingRepository, times(1)).save(newClothing);
        
        System.out.println("✅ Тест 3 пройден: save() сохраняет новый товар");
    }
}