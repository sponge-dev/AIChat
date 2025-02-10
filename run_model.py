#!/usr/bin/env python3
import sys
from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

def main():
    if len(sys.argv) < 2:
        print("[ERROR] Usage: python run_model.py <model_dir>")
        sys.exit(1)

    model_dir = sys.argv[1]
    print(f"[DEBUG] Using model directory: {model_dir}")

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

    device = "cuda" if torch.cuda.is_available() else "cpu"

    # Initialize conversation history list
    conversation_history = []

    print("Starting conversation. Type 'quit' or 'exit' to end.")

    while True:
        # Get user input interactively
        user_input = input("User: ")
        if user_input.lower() in ["quit", "exit"]:
            break

        # Append the new user message to the conversation history
        conversation_history.append(f"User: {user_input}")

        # Build the prompt including all previous messages/responses
        # We add a new "AI:" prompt to cue the model to generate its response.
        prompt = "\n".join(conversation_history) + "\nAI:"
        print(f"[DEBUG] Conversation prompt:\n{prompt}")

        # Tokenize the complete prompt
        inputs = tokenizer(prompt, return_tensors="pt").to(device)
        input_length = inputs.input_ids.shape[1]

        # Generate a response.
        # Setting max_length to input_length + 50 ensures the model generates up to 50 new tokens.
        output = model.generate(
            **inputs,
            max_length=input_length + 50,
            num_return_sequences=1,
            pad_token_id=tokenizer.eos_token_id
        )

        # Since the output contains the entire prompt plus the new tokens,
        # we extract only the newly generated tokens.
        generated_tokens = output[0][input_length:]
        ai_response = tokenizer.decode(generated_tokens, skip_special_tokens=True).strip()

        # Append the AI response to the conversation history
        conversation_history.append(f"AI: {ai_response}")
        print(f"AI: {ai_response}")

if __name__ == "__main__":
    main()
