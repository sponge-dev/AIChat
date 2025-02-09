#!/usr/bin/env python3
import sys
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

def main():
    if len(sys.argv) < 3:
        print("[ERROR] Usage: python run_model.py <model_dir> <user_input>")
        sys.exit(1)

    model_dir = sys.argv[1]      # First argument: model directory
    user_input = sys.argv[2]     # Second argument: user message

    # Debugging info
    print(f"[DEBUG] Using model directory: {model_dir}")
    print(f"[DEBUG] User input: {user_input}")

    # Load tokenizer and model
    try:
        tokenizer = AutoTokenizer.from_pretrained(model_dir)
        model = AutoModelForCausalLM.from_pretrained(
            model_dir,
            torch_dtype=torch.float16,
            device_map="auto"
        )
    except Exception as e:
        print(f"[ERROR] Failed to load model: {e}")
        sys.exit(1)

    # Prepare input and generate response
    device = "cuda" if torch.cuda.is_available() else "cpu"
    inputs = tokenizer(user_input, return_tensors="pt").to(device)
    output = model.generate(
        **inputs,
        max_length=50,
        num_return_sequences=1,
        pad_token_id=tokenizer.eos_token_id
    )

    # Decode and print response (the last print is read by the C++ app)
    response = tokenizer.decode(output[0], skip_special_tokens=True)
    print(f"[DEBUG] AI response: {response}")
    print(response)

if __name__ == "__main__":
    main()
