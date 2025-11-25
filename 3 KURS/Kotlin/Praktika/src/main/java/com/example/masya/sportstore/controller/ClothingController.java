package com.example.masya.sportstore.controller;

import com.example.masya.sportstore.dto.ClothingCreateDto;
import com.example.masya.sportstore.dto.ClothingDto;
import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.entity.Category;
import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.mapper.EntityDtoMapper;
import com.example.masya.sportstore.service.BrandService;
import com.example.masya.sportstore.service.CategoryService;
import com.example.masya.sportstore.service.ClothingService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/api/clothing")
@CrossOrigin(origins = "*")
public class ClothingController {

    private final ClothingService clothingService;
    private final BrandService brandService;
    private final CategoryService categoryService;
    private final EntityDtoMapper mapper;

    @Autowired
    public ClothingController(ClothingService clothingService, 
                            BrandService brandService, 
                            CategoryService categoryService,
                            EntityDtoMapper mapper) {
        this.clothingService = clothingService;
        this.brandService = brandService;
        this.categoryService = categoryService;
        this.mapper = mapper;
    }

    @GetMapping
    public ResponseEntity<List<ClothingDto>> getAllClothing() {
        List<ClothingDto> clothing = clothingService.findAll().stream()
                .map(mapper::toClothingDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(clothing);
    }

    @GetMapping("/{id}")
    public ResponseEntity<ClothingDto> getClothingById(@PathVariable Long id) {
        return clothingService.findById(id)
                .map(mapper::toClothingDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public ResponseEntity<ClothingDto> createClothing(@RequestBody ClothingCreateDto clothingDto) {
        // Fetch brand and category
        Brand brand = brandService.findById(clothingDto.getBrandId())
                .orElseThrow(() -> new RuntimeException("Brand not found with id: " + clothingDto.getBrandId()));
        Category category = categoryService.findById(clothingDto.getCategoryId())
                .orElseThrow(() -> new RuntimeException("Category not found with id: " + clothingDto.getCategoryId()));

        Clothing clothing = mapper.toClothing(clothingDto, brand, category);
        Clothing savedClothing = clothingService.save(clothing);
        return ResponseEntity.ok(mapper.toClothingDto(savedClothing));
    }

    @PutMapping("/{id}")
    public ResponseEntity<ClothingDto> updateClothing(@PathVariable Long id, @RequestBody ClothingCreateDto clothingDto) {
        Brand brand = brandService.findById(clothingDto.getBrandId())
                .orElseThrow(() -> new RuntimeException("Brand not found with id: " + clothingDto.getBrandId()));
        Category category = categoryService.findById(clothingDto.getCategoryId())
                .orElseThrow(() -> new RuntimeException("Category not found with id: " + clothingDto.getCategoryId()));

        Clothing clothing = mapper.toClothing(clothingDto, brand, category);
        clothing.setId(id);
        Clothing updatedClothing = clothingService.update(clothing);
        return ResponseEntity.ok(mapper.toClothingDto(updatedClothing));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteClothing(@PathVariable Long id) {
        clothingService.deleteById(id);
        return ResponseEntity.ok().build();
    }

    @GetMapping("/search")
    public ResponseEntity<List<ClothingDto>> searchClothing(@RequestParam String query) {
        List<ClothingDto> clothing = clothingService.findByModelNameContaining(query).stream()
                .map(mapper::toClothingDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(clothing);
    }

    @GetMapping("/brand/{brandId}")
    public ResponseEntity<List<ClothingDto>> getClothingByBrand(@PathVariable Long brandId) {
        Brand brand = brandService.findById(brandId)
                .orElseThrow(() -> new RuntimeException("Brand not found with id: " + brandId));
        
        List<ClothingDto> clothing = clothingService.findByBrand(brand).stream()
                .map(mapper::toClothingDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(clothing);
    }

    @GetMapping("/category/{categoryId}")
    public ResponseEntity<List<ClothingDto>> getClothingByCategory(@PathVariable Long categoryId) {
        Category category = categoryService.findById(categoryId)
                .orElseThrow(() -> new RuntimeException("Category not found with id: " + categoryId));
        
        List<ClothingDto> clothing = clothingService.findByCategory(category).stream()
                .map(mapper::toClothingDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(clothing);
    }

    @GetMapping("/price-range")
    public ResponseEntity<List<ClothingDto>> getClothingByPriceRange(
            @RequestParam Integer minPrice, 
            @RequestParam Integer maxPrice) {
        List<ClothingDto> clothing = clothingService.findByPriceBetween(minPrice, maxPrice).stream()
                .map(mapper::toClothingDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(clothing);
    }
}