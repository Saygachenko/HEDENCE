// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/UMG/Inventory/Public/PickUpMessageWidget.h"

#include "Components/TextBlock.h"

// Function show message of widget
void UPickUpMessageWidget::ShowMessage(const FText& Text)
{
    if (Message)
    {
        Message->SetText(Text);

        if (!Text.IsEmpty())
        {
            SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            SetVisibility(ESlateVisibility::Hidden);
        }
    }
}