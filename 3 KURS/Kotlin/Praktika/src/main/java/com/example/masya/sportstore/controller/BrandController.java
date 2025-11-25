package com.example.masya.sportstore.controller;

import com.example.masya.sportstore.dto.BrandCreateDto;
import com.example.masya.sportstore.dto.BrandDto;
import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.mapper.EntityDtoMapper;
import com.example.masya.sportstore.service.BrandService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/api/brands")
@CrossOrigin(origins = "*")
public class BrandController {

    private final BrandService brandService;
    private final EntityDtoMapper mapper;

    @Autowired
    public BrandController(BrandService brandService, EntityDtoMapper mapper) {
        this.brandService = brandService;
        this.mapper = mapper;
    }

    @GetMapping
    public ResponseEntity<List<BrandDto>> getAllBrands() {
        List<BrandDto> brands = brandService.findAll().stream()
                .map(mapper::toBrandDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(brands);
    }

    @GetMapping("/{id}")
    public ResponseEntity<BrandDto> getBrandById(@PathVariable Long id) {
        return brandService.findById(id)
                .map(mapper::toBrandDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public ResponseEntity<BrandDto> createBrand(@RequestBody BrandCreateDto brandDto) {
        Brand brand = mapper.toBrand(brandDto);
        Brand savedBrand = brandService.save(brand);
        return ResponseEntity.ok(mapper.toBrandDto(savedBrand));
    }

    @PutMapping("/{id}")
    public ResponseEntity<BrandDto> updateBrand(@PathVariable Long id, @RequestBody BrandCreateDto brandDto) {
        Brand brand = mapper.toBrand(brandDto);
        brand.setId(id);
        Brand updatedBrand = brandService.update(brand);
        return ResponseEntity.ok(mapper.toBrandDto(updatedBrand));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteBrand(@PathVariable Long id) {
        brandService.deleteById(id);
        return ResponseEntity.ok().build();
    }

    @GetMapping("/search")
    public ResponseEntity<List<BrandDto>> searchBrands(@RequestParam String name) {
        List<BrandDto> brands = brandService.findByName(name).stream()
                .map(mapper::toBrandDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(brands);
    }
}