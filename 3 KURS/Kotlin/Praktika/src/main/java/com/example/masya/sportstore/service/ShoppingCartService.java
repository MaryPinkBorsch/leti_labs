package com.example.masya.sportstore.service;

import com.example.masya.sportstore.entity.User;
import com.example.masya.sportstore.entity.Clothing;
import java.util.List;
import java.util.Map;

public interface ShoppingCartService {
    void addToCart(Long userId, Long clothingId, Integer quantity);
    void removeFromCart(Long userId, Long clothingId);
    void updateCartItemQuantity(Long userId, Long clothingId, Integer quantity);
    void clearCart(Long userId);
    Map<Clothing, Integer> getCartItems(Long userId);
    Integer getCartItemsCount(Long userId);
    Double calculateCartTotal(Long userId);
    void mergeCarts(Long temporaryUserId, Long permanentUserId);
    boolean isItemInCart(Long userId, Long clothingId);
}